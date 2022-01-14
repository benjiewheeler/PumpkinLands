project(pumpkinlands)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( pumpkinlands pumpkinlands pumpkinlands.cpp )
target_include_directories( pumpkinlands PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( pumpkinlands ${CMAKE_SOURCE_DIR}/../ricardian )