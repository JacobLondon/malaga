local m = require('maps/mg')

m.header('Default')

m.defencounter('encounter_flyby')
	m.enemy{'leftflyby', shoot='right', speed=7, hp=2, meta=1, level=2}
	m.enemy{'rightflyby', shoot='left', speed=7, hp=2, meta=1, level=2}
for i = 1,15 do
	m.encounter('leftflyby', i*0.25+0.1, 5, 0)
	m.encounter('rightflyby', (i+1)*0.25, 95, 5)
end

m.defencounter('encounter_down')
	m.enemy{'standardsam'}

	m.encounter('standardsam', 0, 30, 0)
	m.encounter('standardsam', 1, 60, 0)
	m.encounter('standardsam', 2, 80, 0)
	m.encounter('standardsam', 4, 20, 0)

m.defencounter('encounter_across')
	m.enemy{'sidewinder_right', move='horzright', speed=4}
	m.enemy{'sidewinder_left', move='horzleft', speed=4}

	m.encounter('sidewinder_left', 0, 100, 30)
	m.encounter('sidewinder_right', 0, 0, 60)
	m.encounter('sidewinder_left', 2, 100, 80)
	m.encounter('sidewinder_right', 2, 0, 20)

m.defencounter('encounter_across_stream')
for i = 1,15 do
	m.encounter('sidewinder_left', i*0.75, 100, 30)
	m.encounter('sidewinder_right', (i+1)*0.75, 0, 10)
end

m.defencounter('encounter1')
	m.enemy{'leftdrifter', move='downleft', speed=4}
	m.enemy{'rightdrifter', move='downright', speed=4}

	m.encounter('leftdrifter', 0, 100, 0)
	m.encounter('leftdrifter', 1, 100, 0)
	m.encounter('rightdrifter', 0, 0, 0)
	m.encounter('rightdrifter', 1, 0, 0)

m.defencounter('encounter2')
	m.enemy{'topstop', move='downsin', speed=4, hp=16, meta=1, level=2}
	m.enemy{'rightstop', move='horzrightstop', shoot='spin', speed=4, hp=8, meta=0.6, level=2}
	m.enemy{'leftstop', move='horzleftstop', shoot='spin', speed=4, hp=8, meta=0.6, level=2}
	m.enemy{'superdood', move='downcos', speed=1, hp=10, meta=1, level=2}

	m.encounter('topstop', 0, 50, 0)
	m.encounter('rightstop', 0, 0, 30)
	m.encounter('rightstop', 1, 0, 60)
	m.encounter('leftstop', 1, 100, 30)
	m.encounter('leftstop', 1, 100, 60)
	m.encounter('topstop', 7, 80, 0)
	m.encounter('topstop', 7, 20, 0)
	m.encounter('topstop', 7, 50, 0)

m.defencounter('stopspiral')
	m.enemy{'spimid', shoot='flower', move='downstop', hp=30, speed=1, meta=5, level=2}
	m.enemy{'spisin', shoot='flower', move='downsin', hp=8, speed=3}
	m.enemy{'spicos', shoot='flower', move='downcos', hp=8, speed=3}

	m.encounter('spimid', 0, 50, 0)
for i = 1,10 do
	m.encounter('spisin', 6 + i*2, 80, 0)
	m.encounter('spicos', 5 + i*2, 20, 0)
end

m.defencounter('doombrigade')
	m.enemy{'doomer', shoot='spin', move='downright', hp=3, speed=6}

for i = 1,6 do
	m.encounter('doomer', i, 40, 0)
	m.encounter('doomer', i, 20, 0)
	m.encounter('doomer', i, 0, 0)
	m.encounter('doomer', i, 0, 20)
	m.encounter('doomer', i, 0, 40)
end

m.defencounter('boss')
	m.enemy{'bossman', png='bossman.png', shoot='flower', move='downstop', hp=300, speed=1, meta=2, level=999}
	m.encounter('bossman', 3, 50, 0)
for i = 1,3 do
	m.encounter('rightstop', i*10, 0, 30 + 15*i)
	m.encounter('leftstop', i*10, 100, 30 + 15*i)
end

m.group('encounter_down')
m.group('encounter_flyby')
m.group('encounter_across_stream')
m.group('encounter1')
m.group('doombrigade')
m.group('stopspiral')
m.group('encounter2')
m.group('boss')

m.generate()
