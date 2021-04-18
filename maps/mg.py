# 
# How to:
# 1. DefineEncounter, then define Enemy's and define Encounter's
# 2. Define Group's where each group name is a DefineEncounter name
# 
# Note:
# An DefineEncounter can have at most 32 Encounters. Subject to change.
# 
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

import sys

enemies = None
encounters = None
groups = None

def Reset():
    global enemies, encounters, groups
    enemies = ["$enemies"]
    encounters = ["$encounters"]
    groups = ["$sets"]
Reset()

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

def DefineEncounter(name):
    encounters.append('[' + name + ']')

class Group:
    def __init__(self, name):
        self.name = name
        groups.append(self)
    def __repr__(self):
        return self.name
    def __str__(self):
        return self.name

def Produce():
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = None

    fp = sys.stdout
    if filename:
        fp = open(filename, "w")

    print("\n".join(str(b) for b in enemies), file=fp)
    print("\n".join(str(b) for b in encounters), file=fp)
    print("\n".join(str(b) for b in groups), file=fp)
    
    if filename:
        fp.close()
