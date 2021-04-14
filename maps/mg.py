
enemies = ["$enemies"]
encounters = ["$encounters"]
groups = ["$sets"]

class Enemy:
    def __init__(self, name, shoot="straight", move="down", hp=5, speed=1, meta=0, level=0, png=None):
        self.name = name
        self.shoot = "bullet_enemy_" + shoot
        self.move = move
        self.hp = hp
        self.speed = speed
        self.meta = meta
        self.level = level
        self.png = png
        enemies.append(self)

    def __repr__(self):
        return """\
(
    name %s
    shoot %s
    move %s
    hp %s
    speed %s
    meta %s
    level %s
%s
)""" % (self.name, self.shoot, self.move, self.hp, self.speed, self.meta, self.level, "" if self.png is None else "    png %s" % self.png)
    def __str__(self):
        return self.__repr__()

class Encounter:
    def __init__(self, name, time, x, y):
        self.name = name
        self.time = time
        self.x = x
        self.y = y
        encounters.append(self)
    def __repr__(self):
        return """\
(
    name %s
    time %s
    x %s
    y %s
)""" % (self.name, self.time, self.x, self.y)
    def __str__(self):
        return self.__repr__()

def encount(name):
    encounters.append('[' + name + ']')

class Group:
    def __init__(self, name):
        self.name = name
        groups.append(self)
    def __repr__(self):
        return self.name
    def __str__(self):
        return self.name

################################################################################
# Enemy Moves:
# - down
# - downleft
# - downright
# - downsin
# - downcos
# - downstop,
# - horzright
# - horzleft
# - horzrightstop
# - horzleftstop
#
# Bullet Types:
# - straight
# - left
# - right
# - spin
# - flower
# - sin
# - sin_wide
# - big
# - beam
#

encount("encounter_flyby")
Enemy('leftflyby', shoot='right', speed=7, hp=2, meta=1, level=2)
Enemy('rightflyby', shoot='left', speed=7, hp=2, meta=1, level=2)
for i in range(15):
    Encounter('leftflyby', i*.25+.1, 5, 0)
    Encounter('rightflyby', (i + 1)*.25, 95, 5)

encount('encounter-down')
Enemy('standardsam')
Encounter('standardsam', 0, 30, 0)
Encounter('standardsam', 1, 60, 0)
Encounter('standardsam', 2, 80, 0)
Encounter('standardsam', 4, 20, 0)

encount('encounter-across')
Enemy('sidewinderRight', move='horzright', speed=4)
Enemy('sidewinderLeft', move='horzleft', speed=4)
Encounter('sidewinderLeft', 0, 100, 30)
Encounter('sidewinderRight', 1, 0, 60)
Encounter('sidewinderLeft', 2, 100, 80)
Encounter('sidewinderRight', 2, 0, 20)

encount('encounter-across-stream')
Enemy('leftdrifter', move='downleft', speed=4)
Enemy('rightdrifter', move='downright', speed=4)
for i in range(15):
    Encounter('sidewinderLeft', .75*i, 100, 30)
    Encounter('sidewinderRight', (i+1)*.75, 0, 10)

encount('encounter1')
Encounter('leftdrifter', 0, 100, 0)
Encounter('leftdrifter', 1, 100, 0)
Encounter('rightdrifter', 0, 0, 0)
Encounter('rightdrifter', 1, 0, 0)

encount('encounter2')
Enemy('topstop', move='downsin', speed=4, hp=16, meta=1, level=2)
Enemy('rightstop', move='horzrightstop', shoot='spin', speed=4, hp=8, meta=1, level=2)
Enemy('leftstop', move='horzleftstop', shoot='spin', speed=4, hp=8, meta=1, level=2)
Enemy('superdood', move='downcos', speed=1, hp=10, meta=1, level=2)
Encounter('topstop', 0, 50, 0)
Encounter('rightstop', 0, 0, 30)
Encounter('rightstop', 1, 0, 60)
Encounter('leftstop', 1, 100, 30)
Encounter('leftstop', 1, 100, 60)
Encounter('topstop', 7, 80, 0)
Encounter('topstop', 7, 20, 0)
Encounter('topstop', 7, 50, 0)

encount('stopspiral')
Enemy('spimid', shoot='flower', move='downstop', hp=30, speed=1, meta=5, level=2)
Enemy('spisin', shoot='flower', move='downsin', hp=8, speed=3)
Enemy('spicos', shoot='flower', move='downcos', hp=8, speed=3)
Encounter('spimid', 0, 50, 0)
for i in range(10):
    Encounter('spisin', 6 + i*2, 80, 0)
    Encounter('spicos', 5 + i*2, 20, 0)

encount('doombrigade')
Enemy('doomer', shoot='spin', move='downright', hp=3, speed=6)
for i in range(6):
    Encounter('doomer', i, 40, 0)
    Encounter('doomer', i, 20, 0)
    Encounter('doomer', i, 0, 0)
    Encounter('doomer', i, 0, 20)
    Encounter('doomer', i, 0, 40)

################################################################################

Group('encounter-down')
Group('encounter_flyby')
Group('encounter-across-stream')
Group('encounter1')
Group('doombrigade')
Group('stopspiral')
Group('encounter2')

print("\n".join(str(b) for b in enemies))
print("\n".join(str(b) for b in encounters))
print("\n".join(str(b) for b in groups))
