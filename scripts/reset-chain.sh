# this script is used to reset the state chain to a fresh state

mkdir -p /var/log/eosio 1>/dev/null 2>&1

pkill -9 keosd
keosd 1>/var/log/eosio/keosd.log 2>&1 &

pkill -9 nodeos
nodeos -e -p eosio \
    --plugin eosio::producer_plugin \
    --plugin eosio::producer_api_plugin \
    --plugin eosio::chain_api_plugin \
    --plugin eosio::http_plugin \
    --plugin eosio::history_plugin \
    --plugin eosio::history_api_plugin \
    --plugin eosio::state_history_plugin \
    --http-server-address=0.0.0.0:8888 \
    --max-transaction-time=1000 \
    --hard-replay \
    --filter-on "*" \
    --delete-all-blocks \
    --disable-replay-opts \
    --access-control-allow-origin='*' \
    --contracts-console \
    --http-validate-host=false \
    --verbose-http-errors 1>/var/log/eosio/nodeos.log 2>&1 &

sleep 2

rm pwd.txt
rm -rf /root/eosio-wallet/

cleos wallet create --file pwd.txt

PASS=$(cat pwd.txt)
cleos wallet open
cleos wallet unlock --password $PASS

cleos wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3  # eosio        account private key
cleos wallet import --private-key 5Ju3RhxeFixCbDxa5NX6KBT8pM4HfgYQC4xSL7wkyNi7WwEqZJq  # pumpkinlands account private key
cleos wallet import --private-key 5HruJk6FLz46psZmVzNutTQ3JnV6TFCtg11ECyANivJkkmPfmE4  # pumpkintoken account private key

cleos create account eosio pumpkinlands EOS5BCMqTkCVwXXZnNRNTB5HLs4X1Ze7CZ1p438isdxRFrKZxLnip # pumpkinlands account public key
cleos create account eosio pumpkintoken EOS7zshmzr63UWashb1hc4APthxSMeNFwRDB1uKdSuPLVUNS3jbnD # pumpkintoken account public key

# test accounts
cleos wallet import --private-key 5K4wc7XGJfLvCafQ3sq5GidziVFpLRb5bjomuoq9sSrp5xd741P  # testaccount1 account private key
cleos wallet import --private-key 5KTN6fyDRxkzSkwJD64Dv4cqTbaACP2LkzGC5sr1LAQ9ybUN2GS  # testaccount2 account private key
cleos wallet import --private-key 5JxLmhe9UDAyuRaxWRd939GaHMH5iNDV6SNbWBkg2AqpWVJEEka  # testaccount3 account private key

cleos create account eosio testaccount1 EOS861WpfT9Mtgt9XbWJz6eXKkr6qyhYQbTtx8BKTAj9AWi7Z8SWo # testaccount1 account public key
cleos create account eosio testaccount2 EOS6KcBBc4FKPC53vtCd4ZH4yUB2kVG8tRUU8v6X1FWxzUtKkwisN # testaccount2 account public key
cleos create account eosio testaccount3 EOS5TL37f1JFvtHvGfTxTi9bpPi7gNWZ6j2pUH6BSYoL98wdi1opu # testaccount3 account public key

cleos set account permission pumpkinlands active --add-code

# deploy the token contract to the local chain
clang-format-10 -i contracts/token/include/*/*.hpp
clang-format-10 -i contracts/token/src/*.cpp

cd contracts/token

if [ ! -f eosio.token.wasm ]; then
    # compile the token contract
    eosio-cpp --abigen -I include src/eosio.token.cpp
fi;

# deploy the compiled contract
cleos set contract pumpkintoken $PWD eosio.token.wasm eosio.token.abi -p pumpkintoken@active

# create a token with an arbitrarily large max supply (1 trillion)
# this max supply will never* be reached so it's irrelevant (*in normal usage)
cleos push action pumpkintoken create '[ "pumpkinlands", "1000000000000.000000 PLT" ]' -p pumpkintoken@active
