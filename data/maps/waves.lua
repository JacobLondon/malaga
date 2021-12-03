local m = require('maps/mg')

m.header('Asteroid')

m.defencounter('wave1')
	m.enemy{'downrightshooter', move='downright', shoot='straight', speed=5, hp=3}
	m.enemy{'downleftshooter', move='downleft', shoot='straight', speed=5, hp=3}
for i = 1,4 do
	if i % 2 == 1 then
		m.encounter('downrightshooter', (i - 1)*2, 0, 0)
		m.encounter('downrightshooter', (i - 1)*2, 25, 0)
		m.encounter('downrightshooter', (i - 1)*2, 50, 0)
		m.encounter('downrightshooter', (i - 1)*2, 75, 0)
	else
		m.encounter('downleftshooter', (i - 1)*2, 10, 0)
		m.encounter('downleftshooter', (i - 1)*2, 35, 0)
		m.encounter('downleftshooter', (i - 1)*2, 65, 0)
		m.encounter('downleftshooter', (i - 1)*2, 85, 0)
	end
end

m.defencounter('wave2')
	m.enemy{'acrossleft', move='horzleft', shoot='sin', speed=8, hp=1, meta=3}
for i = 1,20 do
	m.encounter('acrossleft', (i - 1)* 0.25, 100, 100)
end

m.group('wave1')
m.group('wave2')

m.generate()
