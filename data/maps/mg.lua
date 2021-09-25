--[[
How to:
1. DefineEncounter, then define Enemy's and define Encounter's
2. Define Group's where each group name is a DefineEncounter name

Note:
An DefineEncounter can have at most 32 Encounters. Subject to change.

Enemy Moves:
- down
- downleft
- downright
- downsin
- downcos
- downstop,
- horzright
- horzleft
- horzrightstop
- horzleftstop

Bullet Types:
- straight
- left
- right
- spin
- flower
- sin
- sin_wide
- big
- beam
--]]

local M = {}

function M.New()
	local N = {
		header = {},
		enemies = {'$enemies'},
		encounters = {'$encounters'},
		groups = {'$sets'},
	}
	return N
end

function M.Header(self, atmosphere)
	table.insert(self.header,'**')
	table.insert(self.header, string.format("%s %s", "atmosphere", atmosphere))
	table.insert(self.header, '**')
end

function M.Enemy(self, t)
	setmetatable(t, {__index={
		name=nil,
		shoot='straight',
		move='down',
		hp=5,
		speed=1,
		meta=0,
		level=0,
		png=nil,
	}})
	local name = t[1]
	local shoot = 'bullet_enemy_' .. (t[2] or t.shoot)
	local move = t[3] or t.move
	local hp = t[4] or t.hp
	local speed = t[5] or t.speed
	local meta = t[6] or t.meta
	local level = t[7] or t.level
	local png = t[8] or t.png
	local s = string.format([[
(
name %s
shoot %s
move %s
hp %d
speed %d
meta %f
level %d%s
)]], name, shoot, move, hp, speed, meta,
			level, png and string.format("\npng %s", png) or "")
	table.insert(self.enemies, s)
end

function M.DefineEncounter(self, name)
	table.insert(self.encounters, string.format('[%s]', name))
end

function M.Encounter(self, name, time, x, y)
	local s = string.format([[
(
name %s
time %f
x %d
y %d
)]], name, time, x, y)
	table.insert(self.encounters, s)
end

function M.Group(self, name)
	table.insert(self.groups, name)
end

function M.Generate(self)
	all = {self.header, self.enemies, self.encounters, self.groups}
	for i, t in ipairs(all) do
		for j, v in ipairs(t) do
			print(v)
		end
	end
end

return M
