# add a new fruit config (fruit 0 must stay empty)
cleos push action pumpkinlands setfruit '[1, 10000, 20000, 5, 60]' -p pumpkinlands@active
cleos push action pumpkinlands setfruit '[2, 100000, 160000, 5, 300]' -p pumpkinlands@active

# register new user
cleos push action pumpkinlands regnewuser '["testaccount1"]' -p testaccount1@active
# upgrade the farm
cleos push action pumpkinlands upgradefarm '["testaccount1"]' -p testaccount1@active
# synchronize the farm
cleos push action pumpkinlands syncfarm '["testaccount1", [
    {"event_type": 1, "plot": 0, "fruit": 2, "time": "2022-01-14T21:00:00.000"},
    {"event_type": 1, "plot": 1, "fruit": 2, "time": "2022-01-14T21:00:00.000"},
    {"event_type": 1, "plot": 2, "fruit": 2, "time": "2022-01-14T21:00:00.000"},
    {"event_type": 1, "plot": 3, "fruit": 2, "time": "2022-01-14T21:00:00.000"},
    {"event_type": 1, "plot": 4, "fruit": 2, "time": "2022-01-14T21:00:00.000"},
]]' -p testaccount1@active

# check the table
cleos get table pumpkinlands pumpkinlands accounts
# check the farm state
cleos get table pumpkinlands testaccount1 farmstate -l 100

# check the contract action history
cleos get actions pumpkinlands
