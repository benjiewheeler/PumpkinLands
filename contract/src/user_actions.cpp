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
        row.plot_count = 5;
    });
}
