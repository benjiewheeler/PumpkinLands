/*
    Register a new user and save them in accounts table
    @param {name} user - the name of the account
    @auth caller
*/
ACTION pumpkinlands::regnewuser(const name &user) {
    // check for caller auth
    require_auth(user);

    // get accounts table
    accounts_table accounts(get_self(), get_self().value);
    auto account = accounts.find(user.value);

    // check if the account is not already registered
    check(account == accounts.end(), "user already registered");

    // emplace a new user row
    accounts.emplace(user, [&](auto &row) {
        row.account = user;
        row.balance = extended_asset(asset(0, PLT_SYMBOL), TOKEN_CONTRACT);
        row.farm_size = 5;
    });

    farmstate_table farmstate(get_self(), user.value);

    for (int i = 0; i < 5; i++) {
        // emplace new rows in the farmstate table
        farmstate.emplace(user, [&](auto &row) {
            row.plot = farmstate.available_primary_key();
            row.fruit = 0;
            row.lastHarvest = time_point(microseconds(0));
        });
    }
 }

/*
    Upgrade the user's farm to the next level
    @param {name} user - the name of the account
    @auth caller
*/
ACTION pumpkinlands::upgradefarm(const name &user) {
    // check for caller auth
    require_auth(user);

    // get accounts table
    accounts_table accounts(get_self(), get_self().value);

    // check if the user is registered
    auto account = accounts.require_find(user.value, "user is not registered");

    // check if the farm can be upgraded
    check(account->farm_size < 128, "farm cannot be upgraded anymore");

    auto price = get_land_price(account->farm_size);
    auto ratio = get_market_rate();

    // calculate the upgrade cost
    auto cost = extended_asset(asset(price / ratio, PLT_SYMBOL), TOKEN_CONTRACT);

    // check if the user has enough balance
    check(account->balance >= cost, "insufficient balance");

    // save new farm stats
    accounts.modify(account, same_payer, [&](auto &row) {
        row.farm_size += 3;
        row.balance -= cost;
    });

    farmstate_table farmstate(get_self(), user.value);

    for (int i = 0; i < 3; i++) {
        // emplace new rows in the farmstate table
        farmstate.emplace(user, [&](auto &row) {
            row.plot = farmstate.available_primary_key();
            row.fruit = 0;
            row.lastHarvest = time_point(microseconds(0));
        });
    }

    // burn the upgrade cost
    action(permission_level{get_self(), name("active")}, TOKEN_CONTRACT, name("retire"),
           make_tuple(cost.quantity, std::string("Upgrade Farm - " + user.to_string())))
        .send();
}
