pumpkinlands::accounts_table::const_iterator
pumpkinlands::check_user_registered(const name &user) {
    // get accounts table
    accounts_table accounts(get_self(), get_self().value);

    // check if user is registered
    return accounts.require_find(user.value, "user is not registered");
}
