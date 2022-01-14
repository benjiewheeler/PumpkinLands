#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;

CONTRACT pumpkinlands : public contract {
    // the name of the tokens contract account
    static constexpr eosio::name TOKEN_CONTRACT = name("pumpkintoken");
    // the main token for this game
    static constexpr eosio::symbol PLT_SYMBOL = symbol("PLT", 6);
    // plant/harvest event types
    static constexpr uint8_t PLANT_EVENT = 0;
    static constexpr uint8_t HARVEST_EVENT = 1;
    // empty fruit constant
    static constexpr uint8_t EMPTY_FRUIT = 0;

  public:
    using contract::contract;

    // plant stats struct
    struct farming_event {
        uint8_t event_type;
        uint16_t plot;
        uint8_t fruit;
        time_point_sec time;
    };

    // contract actions
    ACTION setfruit(const uint8_t &fruit, const uint64_t &price, const uint64_t &reward,
                    const uint16_t &farm_size, const uint32_t &harvest_duration);
    ACTION rmfruit(const uint8_t &fruit);

    // user actions
    ACTION regnewuser(const name &user);
    ACTION upgradefarm(const name &user);
    ACTION syncfarm(const name &user, const std::vector<farming_event> &events);

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
        time_point_sec harvest_time;

        auto primary_key() const {
            return plot;
        }
    };

    TABLE fruits_config_entity {
        uint8_t fruit;
        uint64_t price;
        uint64_t reward;
        uint16_t farm_size;
        uint32_t harvest_duration;

        auto primary_key() const {
            return fruit;
        }
    };

    typedef multi_index<name("stat"), currency_stats> stats;
    typedef multi_index<name("accounts"), account_entity> accounts_table;
    typedef multi_index<name("farmstate"), farmstate_entity> farmstate_table;
    typedef multi_index<name("fruitconf"), fruits_config_entity> fruitconf_table;

    // helper functions
    asset get_supply();
    uint64_t get_land_price(const uint16_t &farm_size);
    uint64_t get_market_rate();
    uint64_t quick_pow10(uint64_t n);
};