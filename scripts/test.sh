# register new user
cleos push action pumpkinlands regnewuser '["testaccount1"]' -p testaccount1@active
# upgrade the farm
cleos push action pumpkinlands upgradefarm '["testaccount1"]' -p testaccount1@active

# check the table
cleos get table pumpkinlands pumpkinlands accounts
# check the farm state
cleos get table pumpkinlands testaccount1 farmstate -l 100

# check the contract action history
cleos get actions pumpkinlands
