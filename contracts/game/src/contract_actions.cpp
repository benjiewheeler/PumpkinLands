/*
    Sets a new fruit configuration
    @param {uint8_t} fruit - the id of the fruit
    @param {uint64_t} price - the base price for buying this fruit
    @param {uint64_t} reward - the base price for selling this fruit
    @param {uint16_t} farm_size - the minimum farm size required to unlock this fruit
    @auth caller
*/
ACTION pumpkinlands::setfruit(const uint8_t &fruit_id, const uint64_t &price,
                              const uint64_t &reward, const uint16_t &farm_size,
                              const uint32_t &harvest_duration) {
    // check for contract auth
    require_auth(get_self());

    // get fruits config table
    fruitconf_table fruits(get_self(), get_self().value);
    auto fruit = fruits.find(fruit_id);

    // if there's no config for this fruit
    if (fruit == fruits.end()) {
        fruits.emplace(get_self(), [&](auto &row) {
            row.fruit = fruit_id;
            row.price = price;
            row.reward = reward;
            row.farm_size = farm_size;
            row.harvest_duration = harvest_duration;
        });
    } else {
        // else: modify existing row
        fruits.modify(fruit, same_payer, [&](auto &row) {
            row.fruit = fruit_id;
            row.price = price;
            row.reward = reward;
            row.farm_size = farm_size;
            row.harvest_duration = harvest_duration;
        });
    }
}

/*
    Remove a fruit from the config table
    @param {uint8_t} fruit_id - the id of the fruit to remove
    @auth caller
*/
ACTION pumpkinlands::rmfruit(const uint8_t &fruit_id) {

    // check for contract auth
    require_auth(get_self());

    // get fruits config table
    fruitconf_table fruits(get_self(), get_self().value);
    auto fruit = fruits.require_find(fruit_id, "fruit not found");

    fruits.erase(fruit);
}
