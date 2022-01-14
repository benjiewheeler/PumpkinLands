uint64_t pumpkinlands::quick_pow10(uint64_t n) {
    static int pow10[10] = {1,      10,      100,      1000,      10000,
                            100000, 1000000, 10000000, 100000000, 1000000000};
    return pow10[n];
}

asset pumpkinlands::get_supply() {
    stats statstable(TOKEN_CONTRACT, PLT_SYMBOL.code().raw());
    const auto &st = statstable.get(PLT_SYMBOL.code().raw());
    return st.supply;
}

uint64_t pumpkinlands::get_market_rate() {
    asset supply = get_supply();

    uint8_t decimals = supply.symbol.precision();
    int64_t totalSupply = supply.amount;

    // Less than 100,000 tokens
    if (totalSupply < (100000 * quick_pow10(decimals))) {
        return 1;
    }

    // Less than 500,000 tokens
    if (totalSupply < (500000 * quick_pow10(decimals))) {
        return 5;
    }

    // Less than 1,000,000 tokens
    if (totalSupply < (1000000 * quick_pow10(decimals))) {
        return 10;
    }

    // Less than 5,000,000 tokens
    if (totalSupply < (5000000 * quick_pow10(decimals))) {
        return 50;
    }

    // Less than 10,000,000 tokens
    if (totalSupply < (10000000 * quick_pow10(decimals))) {
        return 100;
    }

    // Less than 50,000,000 tokens
    if (totalSupply < (50000000 * quick_pow10(decimals))) {
        return 500;
    }

    // Less than 100,000,000 tokens
    if (totalSupply < (100000000 * quick_pow10(decimals))) {
        return 1000;
    }

    // Less than 500,000,000 tokens
    if (totalSupply < (500000000 * quick_pow10(decimals))) {
        return 5000;
    }

    // Less than 1,000,000,000 tokens
    if (totalSupply < (1000000000 * quick_pow10(decimals))) {
        return 10000;
    }

    // Linear growth from here
    return totalSupply / 10000;
}

uint64_t pumpkinlands::get_land_price(const uint16_t &farm_size) {
    uint8_t decimals = PLT_SYMBOL.precision();
    uint64_t multiplier = quick_pow10(decimals);

    if (farm_size <= 5) {
        return 1 * multiplier;
    } else if (farm_size <= 8) {
        return 50 * multiplier;
    } else if (farm_size <= 11) {
        return 500 * multiplier;
    }

    return 2500 * multiplier;
}
