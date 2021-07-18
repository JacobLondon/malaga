# MG: MyGalaga
Space shooter, galaga-like game. Note that I don't own these assets, and they will not be available for download.

![](https://user-images.githubusercontent.com/17059471/126079271-e560e05b-bd8f-429b-96b5-016c79d9e1de.png)

# Features
* Backgrounds are simple to create and customize using the scene_man.c library in atmos.c module.
* Levels are fully scripted. Scripts are generated with maps/mg.py. Use maps/test.py as a reference, this will generate maps/test.mg which will be then available from the level select screen.
* Gamemodes: Endless, Double-Trouble, Godmode
* Combos, multipliers, rounds, health
* Enemy explosions when sprites are available
* Statically compiled
* Default sprites if some or all are missing

# TODO
* Make more levels
* Powerups and drops
* Level select with high score stored in level file
* Flashlight mode
* Make new so_cb_strobe
  * and a new mode called solar flare mode
* Level files can define assets and which background to use
* Don't impound .mg files but the python files instead
  * Easier way to generate all the .mg files

# Raylib Installation
* Linux: libraylib.a into /usr/local/lib/
* Windows: Follow Windows installer instruction on the Raylib releases page
