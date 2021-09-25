
local mg = require('maps/mg')

local N = mg.New()

mg.Header(N, 'Default')

mg.DefineEncounter(N, 'encounter_flyby')
	mg.Enemy(N, {'leftflyby', shoot='right', speed=7, hp=2, meta=1, level=2})
	mg.Enemy(N, {'rightflyby', shoot='left', speed=7, hp=2, meta=1, level=2})
for i = 1,15 do
	mg.Encounter(N, 'leftflyby', i*0.25+0.1, 5, 0)
	mg.Encounter(N, 'rightflyby', (i+1)*0.25, 95, 5)
end

mg.DefineEncounter(N, 'encounter_down')
	mg.Enemy(N, {'standardsam'})

	mg.Encounter(N, 'standardsam', 0, 30, 0)
	mg.Encounter(N, 'standardsam', 1, 60, 0)
	mg.Encounter(N, 'standardsam', 2, 80, 0)
	mg.Encounter(N, 'standardsam', 4, 20, 0)

mg.DefineEncounter(N, 'encounter_across')
	mg.Enemy(N, {'sidewinder_right', move='horzright', speed=4})
	mg.Enemy(N, {'sidewinder_left', move='horzleft', speed=4})

	mg.Encounter(N, 'sidewinder_left', 0, 100, 30)
	mg.Encounter(N, 'sidewinder_right', 0, 0, 60)
	mg.Encounter(N, 'sidewinder_left', 2, 100, 80)
	mg.Encounter(N, 'sidewinder_right', 2, 0, 20)

mg.DefineEncounter(N, 'encounter_across_stream')
for i = 1,15 do
	mg.Encounter(N, 'sidewinder_left', i*0.75, 100, 30)
	mg.Encounter(N, 'sidewinder_right', (i+1)*0.75, 0, 10)
end

mg.DefineEncounter(N, 'encounter1')
	mg.Enemy(N, {'leftdrifter', move='downleft', speed=4})
	mg.Enemy(N, {'rightdrifter', move='downright', speed=4})

	mg.Encounter(N, 'leftdrifter', 0, 100, 0)
	mg.Encounter(N, 'leftdrifter', 1, 100, 0)
	mg.Encounter(N, 'rightdrifter', 0, 0, 0)
	mg.Encounter(N, 'rightdrifter', 1, 0, 0)

mg.DefineEncounter(N, 'encounter2')
	mg.Enemy(N, {'topstop', move='downsin', speed=4, hp=16, meta=1, level=2})
	mg.Enemy(N, {'rightstop', move='horzrightstop', shoot='spin', speed=4, hp=8, meta=0.6, level=2})
	mg.Enemy(N, {'leftstop', move='horzleftstop', shoot='spin', speed=4, hp=8, meta=0.6, level=2})
	mg.Enemy(N, {'superdood', move='downcos', speed=1, hp=10, meta=1, level=2})

	mg.Encounter(N, 'topstop', 0, 50, 0)
	mg.Encounter(N, 'rightstop', 0, 0, 30)
	mg.Encounter(N, 'rightstop', 1, 0, 60)
	mg.Encounter(N, 'leftstop', 1, 100, 30)
	mg.Encounter(N, 'leftstop', 1, 100, 60)
	mg.Encounter(N, 'topstop', 7, 80, 0)
	mg.Encounter(N, 'topstop', 7, 20, 0)
	mg.Encounter(N, 'topstop', 7, 50, 0)

mg.DefineEncounter(N, 'stopspiral')
	mg.Enemy(N, {'spimid', shoot='flower', move='downstop', hp=30, speed=1, meta=5, level=2})
	mg.Enemy(N, {'spisin', shoot='flower', move='downsin', hp=8, speed=3})
	mg.Enemy(N, {'spicos', shoot='flower', move='downcos', hp=8, speed=3})

	mg.Encounter(N, 'spimid', 0, 50, 0)
for i = 1,10 do
	mg.Encounter(N, 'spisin', 6 + i*2, 80, 0)
	mg.Encounter(N, 'spicos', 5 + i*2, 20, 0)
end

mg.DefineEncounter(N, 'doombrigade')
	mg.Enemy(N, {'doomer', shoot='spin', move='downright', hp=3, speed=6})

for i = 1,6 do
	mg.Encounter(N, 'doomer', i, 40, 0)
	mg.Encounter(N, 'doomer', i, 20, 0)
	mg.Encounter(N, 'doomer', i, 0, 0)
	mg.Encounter(N, 'doomer', i, 0, 20)
	mg.Encounter(N, 'doomer', i, 0, 40)
end


mg.Group(N, 'encounter_down')
mg.Group(N, 'encounter_flyby')
mg.Group(N, 'encounter_across_stream')
mg.Group(N, 'encounter1')
mg.Group(N, 'doombrigade')
mg.Group(N, 'stopspiral')
mg.Group(N, 'encounter2')

mg.Generate(N)
