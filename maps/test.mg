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
    name standardsam
    shoot bullet_enemy_straight
    move down
    hp 3
    speed 1
    level 0
)
(
    name sidewinderRight
    shoot bullet_enemy_straight
    move horzright
    hp 3
    speed 4
    level 0
)
(
    name sidewinderLeft
    shoot bullet_enemy_straight
    move horzleft
    hp 3
    speed 4
    level 0
)
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
    move downsin
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
    level 1
)
(
    name leftstop
    shoot bullet_enemy_spin
    move horzleftstop
    hp 8
    speed 3
    meta 1
    level 0
)
(
    name superdood
    shoot bullet_enemy_straight
    move downcos
    hp 10
    speed 1
    meta 1
    level 2
)
(
    name leftflyby
    shoot bullet_enemy_right
    move down
    hp 2
    speed 7
    level 0
)
(
    name rightflyby
    shoot bullet_enemy_left
    move down
    hp 2
    speed 7
    level 0
)

$encounters
[encounter_flyby]
(
    name leftflyby
    time 0
    x 5
    y 0
)
(
    name rightflyby
    time 0.25
    x 95
    y 0
)
(
    name leftflyby
    time 0.5
    x 5
    y 0
)
(
    name rightflyby
    time 0.75
    x 95
    y 0
)
(
    name leftflyby
    time 1
    x 5
    y 0
)
(
    name rightflyby
    time 1.25
    x 95
    y 0
)
(
    name leftflyby
    time 1.5
    x 5
    y 0
)
(
    name rightflyby
    time 1.75
    x 95
    y 0
)
(
    name leftflyby
    time 2
    x 5
    y 0
)
(
    name rightflyby
    time 2.25
    x 95
    y 0
)
(
    name leftflyby
    time 2.5
    x 5
    y 0
)
(
    name rightflyby
    time 2.75
    x 95
    y 0
)
(
    name leftflyby
    time 3
    x 5
    y 0
)
(
    name rightflyby
    time 3.25
    x 95
    y 0
)
(
    name leftflyby
    time 3.5
    x 5
    y 0
)
(
    name rightflyby
    time 3.75
    x 95
    y 0
)
(
    name leftflyby
    time 4
    x 5
    y 0
)
(
    name rightflyby
    time 4.25
    x 95
    y 0
)
[encounter-down]
(
    name standardsam
    time 0
    x 30
    y 0
)
(
    name standardsam
    time 1
    x 60
    y 0
)
(
    name standardsam
    time 2
    x 80
    y 0
)
(
    name standardsam
    time 2
    x 20
    y 0
)
[encounter-across]
(
    name sidewinderLeft
    time 0
    x 100
    y 30
)
(
    name sidewinderRight
    time 1
    x 0
    y 60
)
(
    name sidewinderLeft
    time 2
    x 100
    y 80
)
(
    name sidewinderRight
    time 2
    x 0
    y 20
)
[encounter-across-stream]
(
    name sidewinderLeft
    time 0
    x 100
    y 30
)
(
    name sidewinderRight
    time 0
    x 0
    y 10
)
(
    name sidewinderLeft
    time 0.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 0.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 1
    x 100
    y 30
)
(
    name sidewinderRight
    time 1
    x 0
    y 10
)
(
    name sidewinderLeft
    time 1.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 1.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 2
    x 100
    y 30
)
(
    name sidewinderRight
    time 2
    x 0
    y 10
)
(
    name sidewinderLeft
    time 2.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 2.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 3
    x 100
    y 30
)
(
    name sidewinderRight
    time 3
    x 0
    y 10
)
(
    name sidewinderLeft
    time 3.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 3.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 4
    x 100
    y 30
)
(
    name sidewinderRight
    time 4
    x 0
    y 10
)
(
    name sidewinderLeft
    time 4.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 4.5
    x 0
    y 10
)
[encounter1]
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
[encounter2]
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
encounter-down
#encounter_flyby
#encounter-across-stream
#encounter1
#encounter2
