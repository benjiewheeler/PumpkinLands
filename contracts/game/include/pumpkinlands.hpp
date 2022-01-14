#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT pumpkinlands : public contract {
    // the name of the tokens contract account
    static constexpr eosio::name TOKEN_CONTRACT = name("pumpkintoken");
    // the main token for this game
    static constexpr eosio::symbol PLT_SYMBOL = symbol("PLT", 4);

  public:
    using contract::contract;

    // user actions
    ACTION regnewuser(const name &user);
    ACTION upgradefarm(const name &user);

  private:
    // token stats struct
    struct currency_stats {
        asset supply;
        asset max_supply;
        name issuer;

        uint64_t primary_key() const {
            return supply.symbol.code().raw();
        }
    };

    TABLE account_entity {
        name account;
        uint16_t farm_size;
        extended_asset balance;

        auto primary_key() const {
            return account.value;
        }
    };

    TABLE farmstate_entity {
        uint16_t plot;
        uint8_t fruit;
        time_point lastHarvest;

        auto primary_key() const {
            return plot;
        }
    };

    typedef multi_index<name("stat"), currency_stats> stats;
    typedef multi_index<name("accounts"), account_entity> accounts_table;
    typedef multi_index<name("farmstate"), farmstate_entity> farmstate_table;

    // helper functions
    asset get_supply();
    uint64_t get_land_price(const uint16_t &farm_size);
    uint64_t get_market_rate();
    uint64_t quick_pow10(uint64_t n);
};