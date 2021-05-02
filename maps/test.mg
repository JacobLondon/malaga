$enemies
(
    name leftflyby
    shoot bullet_enemy_right
    move down
    hp 2
    speed 7
    meta 1
    level 2

)
(
    name rightflyby
    shoot bullet_enemy_left
    move down
    hp 2
    speed 7
    meta 1
    level 2

)
(
    name standardsam
    shoot bullet_enemy_straight
    move down
    hp 5
    speed 1
    meta 0
    level 0

)
(
    name sidewinderRight
    shoot bullet_enemy_straight
    move horzright
    hp 5
    speed 4
    meta 0
    level 0

)
(
    name sidewinderLeft
    shoot bullet_enemy_straight
    move horzleft
    hp 5
    speed 4
    meta 0
    level 0

)
(
    name leftdrifter
    shoot bullet_enemy_straight
    move downleft
    hp 5
    speed 4
    meta 0
    level 0

)
(
    name rightdrifter
    shoot bullet_enemy_straight
    move downright
    hp 5
    speed 4
    meta 0
    level 0

)
(
    name topstop
    shoot bullet_enemy_straight
    move downsin
    hp 16
    speed 4
    meta 1
    level 2

)
(
    name rightstop
    shoot bullet_enemy_spin
    move horzrightstop
    hp 8
    speed 4
    meta 1
    level 2

)
(
    name leftstop
    shoot bullet_enemy_spin
    move horzleftstop
    hp 8
    speed 4
    meta 1
    level 2

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
    name spimid
    shoot bullet_enemy_flower
    move downstop
    hp 30
    speed 1
    meta 5
    level 2

)
(
    name spisin
    shoot bullet_enemy_flower
    move downsin
    hp 8
    speed 3
    meta 0
    level 0

)
(
    name spicos
    shoot bullet_enemy_flower
    move downcos
    hp 8
    speed 3
    meta 0
    level 0

)
(
    name doomer
    shoot bullet_enemy_spin
    move downright
    hp 3
    speed 6
    meta 0
    level 0

)
$encounters
[encounter_flyby]
(
    name leftflyby
    time 0.1
    x 5
    y 0
)
(
    name rightflyby
    time 0.25
    x 95
    y 5
)
(
    name leftflyby
    time 0.35
    x 5
    y 0
)
(
    name rightflyby
    time 0.5
    x 95
    y 5
)
(
    name leftflyby
    time 0.6
    x 5
    y 0
)
(
    name rightflyby
    time 0.75
    x 95
    y 5
)
(
    name leftflyby
    time 0.85
    x 5
    y 0
)
(
    name rightflyby
    time 1.0
    x 95
    y 5
)
(
    name leftflyby
    time 1.1
    x 5
    y 0
)
(
    name rightflyby
    time 1.25
    x 95
    y 5
)
(
    name leftflyby
    time 1.35
    x 5
    y 0
)
(
    name rightflyby
    time 1.5
    x 95
    y 5
)
(
    name leftflyby
    time 1.6
    x 5
    y 0
)
(
    name rightflyby
    time 1.75
    x 95
    y 5
)
(
    name leftflyby
    time 1.85
    x 5
    y 0
)
(
    name rightflyby
    time 2.0
    x 95
    y 5
)
(
    name leftflyby
    time 2.1
    x 5
    y 0
)
(
    name rightflyby
    time 2.25
    x 95
    y 5
)
(
    name leftflyby
    time 2.35
    x 5
    y 0
)
(
    name rightflyby
    time 2.5
    x 95
    y 5
)
(
    name leftflyby
    time 2.6
    x 5
    y 0
)
(
    name rightflyby
    time 2.75
    x 95
    y 5
)
(
    name leftflyby
    time 2.85
    x 5
    y 0
)
(
    name rightflyby
    time 3.0
    x 95
    y 5
)
(
    name leftflyby
    time 3.1
    x 5
    y 0
)
(
    name rightflyby
    time 3.25
    x 95
    y 5
)
(
    name leftflyby
    time 3.35
    x 5
    y 0
)
(
    name rightflyby
    time 3.5
    x 95
    y 5
)
(
    name leftflyby
    time 3.6
    x 5
    y 0
)
(
    name rightflyby
    time 3.75
    x 95
    y 5
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
    time 4
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
    time 0.0
    x 100
    y 30
)
(
    name sidewinderRight
    time 0.75
    x 0
    y 10
)
(
    name sidewinderLeft
    time 0.75
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
    time 1.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 2.25
    x 0
    y 10
)
(
    name sidewinderLeft
    time 2.25
    x 100
    y 30
)
(
    name sidewinderRight
    time 3.0
    x 0
    y 10
)
(
    name sidewinderLeft
    time 3.0
    x 100
    y 30
)
(
    name sidewinderRight
    time 3.75
    x 0
    y 10
)
(
    name sidewinderLeft
    time 3.75
    x 100
    y 30
)
(
    name sidewinderRight
    time 4.5
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
    time 5.25
    x 0
    y 10
)
(
    name sidewinderLeft
    time 5.25
    x 100
    y 30
)
(
    name sidewinderRight
    time 6.0
    x 0
    y 10
)
(
    name sidewinderLeft
    time 6.0
    x 100
    y 30
)
(
    name sidewinderRight
    time 6.75
    x 0
    y 10
)
(
    name sidewinderLeft
    time 6.75
    x 100
    y 30
)
(
    name sidewinderRight
    time 7.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 7.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 8.25
    x 0
    y 10
)
(
    name sidewinderLeft
    time 8.25
    x 100
    y 30
)
(
    name sidewinderRight
    time 9.0
    x 0
    y 10
)
(
    name sidewinderLeft
    time 9.0
    x 100
    y 30
)
(
    name sidewinderRight
    time 9.75
    x 0
    y 10
)
(
    name sidewinderLeft
    time 9.75
    x 100
    y 30
)
(
    name sidewinderRight
    time 10.5
    x 0
    y 10
)
(
    name sidewinderLeft
    time 10.5
    x 100
    y 30
)
(
    name sidewinderRight
    time 11.25
    x 0
    y 10
)
[encounter1]
(
    name leftdrifter
    time 0
    x 100
    y 0
)
(
    name leftdrifter
    time 1
    x 100
    y 0
)
(
    name rightdrifter
    time 0
    x 0
    y 0
)
(
    name rightdrifter
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
    time 0
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
(
    name topstop
    time 7
    x 80
    y 0
)
(
    name topstop
    time 7
    x 20
    y 0
)
(
    name topstop
    time 7
    x 50
    y 0
)
[stopspiral]
(
    name spimid
    time 0
    x 50
    y 0
)
(
    name spisin
    time 6
    x 80
    y 0
)
(
    name spicos
    time 5
    x 20
    y 0
)
(
    name spisin
    time 8
    x 80
    y 0
)
(
    name spicos
    time 7
    x 20
    y 0
)
(
    name spisin
    time 10
    x 80
    y 0
)
(
    name spicos
    time 9
    x 20
    y 0
)
(
    name spisin
    time 12
    x 80
    y 0
)
(
    name spicos
    time 11
    x 20
    y 0
)
(
    name spisin
    time 14
    x 80
    y 0
)
(
    name spicos
    time 13
    x 20
    y 0
)
(
    name spisin
    time 16
    x 80
    y 0
)
(
    name spicos
    time 15
    x 20
    y 0
)
(
    name spisin
    time 18
    x 80
    y 0
)
(
    name spicos
    time 17
    x 20
    y 0
)
(
    name spisin
    time 20
    x 80
    y 0
)
(
    name spicos
    time 19
    x 20
    y 0
)
(
    name spisin
    time 22
    x 80
    y 0
)
(
    name spicos
    time 21
    x 20
    y 0
)
(
    name spisin
    time 24
    x 80
    y 0
)
(
    name spicos
    time 23
    x 20
    y 0
)
[doombrigade]
(
    name doomer
    time 0
    x 40
    y 0
)
(
    name doomer
    time 0
    x 20
    y 0
)
(
    name doomer
    time 0
    x 0
    y 0
)
(
    name doomer
    time 0
    x 0
    y 20
)
(
    name doomer
    time 0
    x 0
    y 40
)
(
    name doomer
    time 1
    x 40
    y 0
)
(
    name doomer
    time 1
    x 20
    y 0
)
(
    name doomer
    time 1
    x 0
    y 0
)
(
    name doomer
    time 1
    x 0
    y 20
)
(
    name doomer
    time 1
    x 0
    y 40
)
(
    name doomer
    time 2
    x 40
    y 0
)
(
    name doomer
    time 2
    x 20
    y 0
)
(
    name doomer
    time 2
    x 0
    y 0
)
(
    name doomer
    time 2
    x 0
    y 20
)
(
    name doomer
    time 2
    x 0
    y 40
)
(
    name doomer
    time 3
    x 40
    y 0
)
(
    name doomer
    time 3
    x 20
    y 0
)
(
    name doomer
    time 3
    x 0
    y 0
)
(
    name doomer
    time 3
    x 0
    y 20
)
(
    name doomer
    time 3
    x 0
    y 40
)
(
    name doomer
    time 4
    x 40
    y 0
)
(
    name doomer
    time 4
    x 20
    y 0
)
(
    name doomer
    time 4
    x 0
    y 0
)
(
    name doomer
    time 4
    x 0
    y 20
)
(
    name doomer
    time 4
    x 0
    y 40
)
(
    name doomer
    time 5
    x 40
    y 0
)
(
    name doomer
    time 5
    x 20
    y 0
)
(
    name doomer
    time 5
    x 0
    y 0
)
(
    name doomer
    time 5
    x 0
    y 20
)
(
    name doomer
    time 5
    x 0
    y 40
)
$sets
encounter-down
encounter_flyby
encounter-across-stream
encounter1
doombrigade
stopspiral
encounter2
