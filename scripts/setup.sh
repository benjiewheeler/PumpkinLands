# this script is used to initiate the smart contract environment
# it installs the eosio CDT software and initiates an empty contract

if [ ! $(which eosio-cpp) ]; then
    wget -nc https://github.com/EOSIO/eos/releases/download/v2.0.13/eosio_2.0.13-1-ubuntu-18.04_amd64.deb
    wget -nc https://github.com/EOSIO/eosio.cdt/releases/download/v1.7.0/eosio.cdt_1.7.0-1-ubuntu-18.04_amd64.deb
    apt update
    apt install -y ./eosio_2.0.13-1-ubuntu-18.04_amd64.deb
    apt install -y ./eosio.cdt_1.7.0-1-ubuntu-18.04_amd64.deb
    apt install -y clang-format-10
    rm -rf ./eosio_2.0.13-1-ubuntu-18.04_amd64.deb
    rm -rf ./eosio.cdt_1.7.0-1-ubuntu-18.04_amd64.deb
fi;

if [ ! -d contract  ]; then
	eosio-init --project pumpkinlands
	mv pumpkinlands contract
fi;