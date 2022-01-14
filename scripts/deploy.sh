# this script is used to deploy the compiled wasm to the local chain

cd contracts/game

if [ ! -f pumpkinlands.wasm ]; then
    echo "no wasm file found"
    exit 1
fi;

# deploy the compiled contract
cleos set contract pumpkinlands $PWD pumpkinlands.wasm pumpkinlands.abi -p pumpkinlands@active

# check for the ram usage
cleos get account pumpkinlands | grep quota
cleos get account pumpkinlands -j | grep ram_usage
