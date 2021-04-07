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
# - bullet_enemy_straight
# - bullet_enemy_left
# - bullet_enemy_right
# - bullet_enemy_spin
# - bullet_enemy_flower
# - bullet_player_straight
# - bullet_player_left
# - bullet_player_right
# - bullet_player_spin
# - bullet_player_flower
# - bullet_enemy_sin
# - bullet_enemy_sin_wide
# - bullet_enemy_sin
# - bullet_player_sin_wide
# - bullet_enemy_big
# - bullet_player_big
# - bullet_enemy_beam
# - bullet_player_beam
# - bullet_player_parabola
#

$enemies
(
    name left_drifter
    shoot bullet_enemy_straight
    move downleft
    hp 3
    speed 5
    level 0
)
(
    name right_drifter
    shoot bullet_enemy_straight
    move downright
    hp 3
    speed 5
    level 0
)
(
    name topstop
    shoot bullet_enemy_spin
    move downstop
    hp 8
    speed 5
    meta 1
    level 2
)
(
    name rightstop
    shoot bullet_enemy_spin
    move horzrightstop
    hp 8
    speed 3
    meta 1
    level 2
)
(
    name leftstop
    shoot bullet_enemy_spin
    move horzleftstop
    hp 8
    speed 3
    meta 1
    level 2
)

$encounters
[encounter0]
(
    name left_drifter
    time 0
    x 100
    y 0
)
(
    name left_drifter
    time 1
    x 100
    y 0
)
(
    name right_drifter
    time 0
    x 0
    y 0
)
(
    name right_drifter
    time 1
    x 0
    y 0
)
[encounter1]
(
    name topstop
    time 0
    x 50
    y 0
)
(
    name rightstop
    time 1
    x 0
    y 30
)
(
    name rightstop
    time 1
    x 0
    y 60
)
(
    name leftstop
    time 1
    x 100
    y 30
)
(
    name leftstop
    time 1
    x 100
    y 60
)

$sets
encounter0
encounter1
