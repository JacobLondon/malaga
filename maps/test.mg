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
    speed 10
    level 0
)

$encounters
[encounter0]
(
    name left_drifter
    time 2
    x 100
    y 0
)
(
    name left_drifter
    time 4
    x 100
    y 0
)
[encounter1]
(
    name left_drifter
    time 2
    x 100
    y 0
)

$sets
encounter0
encounter1
