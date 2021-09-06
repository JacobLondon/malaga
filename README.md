# MG: MyGalaga
Space shooter, galaga-like game. Note that I don't own these assets, and they will not be available for download.

![](https://user-images.githubusercontent.com/17059471/126079271-e560e05b-bd8f-429b-96b5-016c79d9e1de.png)

# Features
* Xbox Controller Support
* Backgrounds are simple to create and customize using the scene_man.c library in atmos.c module.
* Levels are fully scripted. Scripts are generated with maps/mg.py. Use maps/test.py as a reference, this will generate maps/test.mg which will be then available from the level select screen.
* Gamemodes: Endless, Double-Trouble, Godmode
* Combos, multipliers, rounds, health
* Enemy explosions when sprites are available
* Statically compiled
* Default sprites if some or all are missing

# Compiling
```bash
make
./test[.exe]
```

# TODO
* Flower which spins
* Make more levels
* Powerups and drops
* Level select with high score stored in level file
* Flashlight mode
* Make new so_cb_strobe
  * and a new mode called solar flare mode
* Level files can define assets and which background to use
* Don't impound .mg files but the python files instead
  * Easier way to generate all the .mg files

# Requirements
* Linux
  * make, gcc, /usr/local/lib/libraylib.a
* Windows
  * make, tcc/Raylib installation

# Raylib Installation
* Linux: Manually clone/build Raylib and `cp libraylib.a /usr/local/lib/` (NOTE: ln -s won't work)
* Windows: Follow Windows exe installer instructions on the Raylib releases [page](https://github.com/raysan5/raylib/releases)
  * New versions of Raylib may be different. This may require performing the exe install and changing `configure.py` Windows section's CC/CFLAGS/LDFLAGS.
  * Steps for setting compiler flags are at the Raylib GitHub page
