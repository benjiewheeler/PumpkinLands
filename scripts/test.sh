# register new user
cleos push action pumpkinlands regnewuser '["testaccount1"]' -p testaccount1@active

# check the table
cleos get table pumpkinlands pumpkinlands accounts

