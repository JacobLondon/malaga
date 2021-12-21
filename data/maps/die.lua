local m = require('maps/mg')

m.header('Default')

m.defencounter('break1')
	m.enemy{'breakerA', shoot='flower', move='down', level=4}
for i = 1,3 do
	m.encounter('breakerA', i, 5, 0)
	m.encounter('breakerA', (i + 1), 95, 0)
end

m.group('break1')

m.generate()
