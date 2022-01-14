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

  private:
    TABLE account_entity {
        name account;
        uint16_t plot_count;
        extended_asset balance;

        auto primary_key() const {
            return account.value;
        }
    };

    typedef multi_index<name("accounts"), account_entity> accounts_table;

    // helper functions
    accounts_table::const_iterator check_user_registered(const name &user);
};