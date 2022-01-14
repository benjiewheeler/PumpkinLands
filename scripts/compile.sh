# this script is used to format the source code, and to compile the contract, then deploy it to the local chain

clang-format-10 -i contracts/game/include/*.hpp
clang-format-10 -i contracts/game/src/*.cpp

cd contracts/game

eosio-cpp --abigen -I include -o pumpkinlands.wasm src/pumpkinlands.cpp

if [ -f pumpkinlands.wasm ]; then
    # deploy the compiled contract
    cleos set contract pumpkinlands $PWD pumpkinlands.wasm pumpkinlands.abi -p pumpkinlands@active

    # check for the ram usage
    cleos get account pumpkinlands | grep quota
    cleos get account pumpkinlands -j | grep ram_usage
fi;

