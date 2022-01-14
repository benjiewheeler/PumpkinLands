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
            row.fruit = 1; // the first available fruit
            row.harvest_time = time_point_sec(0);
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
            row.harvest_time = time_point_sec(0);
        });
    }

    // burn the upgrade cost
    action(permission_level{get_self(), name("active")}, TOKEN_CONTRACT, name("retire"),
           make_tuple(cost.quantity, std::string("Upgrade Farm - " + user.to_string())))
        .send();
}

/*
    Synchronize and save the user's farm state
    @param {name} user - the name of the account
    @param {vector<farming_event>} events - the list of events to save to the blockchain
    @auth caller
*/
ACTION pumpkinlands::syncfarm(const name &user, const std::vector<farming_event> &events) {
    // check for caller auth
    require_auth(user);

    // check for empty list of events
    check(events.size() > 0, "events cannot be empty");

    // get accounts table
    accounts_table accounts(get_self(), get_self().value);

    // check if the user is registered
    auto account = accounts.require_find(user.value, "user is not registered");
    auto account_balance = account->balance.quantity;

    auto ratio = get_market_rate();

    // get fruits config table
    fruitconf_table fruits(get_self(), get_self().value);

    // get farm state table
    farmstate_table farmstate(get_self(), user.value);

    for (std::size_t i = 0; i != events.size(); ++i) {
        const farming_event &ev = events[i];
        check(ev.time <= current_time_point(), "event is in the future");
        check(ev.event_type == PLANT_EVENT || ev.event_type == HARVEST_EVENT, "invalid event");

        if (i > 0) {
            check(ev.time >= events[i - 1].time, "invalid events order");
        }

        auto plot = farmstate.require_find(ev.plot, "invalid plot");

        if (ev.event_type == PLANT_EVENT) {
            // check if the fruit is a valid one
            auto fruit = fruits.require_find(ev.fruit, "invalid fruit");

            check(plot->fruit == EMPTY_FRUIT, "plot is not empty");
            check(account->farm_size >= fruit->farm_size, "insufficient farm level");
            check(account_balance >= asset(fruit->price / ratio, PLT_SYMBOL),
                  "insufficient balance");

            // save the plot state
            farmstate.modify(plot, same_payer, [&](auto &row) {
                row.fruit = ev.fruit;
                row.harvest_time = time_point_sec(current_time_point().sec_since_epoch() +
                                                  fruit->harvest_duration);
            });

            // deduct the cost from the balance
            account_balance -= asset(fruit->price / ratio, PLT_SYMBOL);
        }

        if (ev.event_type == HARVEST_EVENT) {
            check(plot->fruit != EMPTY_FRUIT, "no fruit found");
            check(plot->harvest_time <= ev.time, "fruit not ripe");

            // get the previously planted fruit
            auto fruit = fruits.require_find(plot->fruit, "invalid fruit");

            // save the plot state
            farmstate.modify(plot, same_payer, [&](auto &row) { row.fruit = EMPTY_FRUIT; });

            // adde the reward to the balance
            account_balance += asset(fruit->reward / ratio, PLT_SYMBOL);
        }
    }

    int64_t balance_diff = account_balance.amount - account->balance.quantity.amount;

    // save the new user balance
    accounts.modify(account, same_payer, [&](auto &row) {
        row.balance = extended_asset(account_balance, TOKEN_CONTRACT);
    });

    if (balance_diff > 0) {
        // issue the gained difference cost
        action(permission_level{get_self(), name("active")}, TOKEN_CONTRACT, name("issue"),
               make_tuple(get_self(), asset(balance_diff, PLT_SYMBOL),
                          std::string("Farming - " + user.to_string())))
            .send();
    } else if (balance_diff < 0) {
        // burn the farming cost
        action(permission_level{get_self(), name("active")}, TOKEN_CONTRACT, name("retire"),
               make_tuple(asset(abs(balance_diff), PLT_SYMBOL),
                          std::string("Farming - " + user.to_string())))
            .send();
    }
}
