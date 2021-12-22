# Malaga
Space shooter, galaga-like game. Note that I don't own these assets, and they will not be available for download.

```bash
git clone git@github.com:/JacobLondon/malaga --recurse-submodules
```

![](https://user-images.githubusercontent.com/17059471/147126396-6eacfa7b-8010-4b27-a970-b1b2ecd106d4.png)

# Features
* Xbox Controller Support (non-menu, just in-game)
* Backgrounds are simple to create and customize using the scene_man.c library in atmos.c module. (Planning for backgrounds to be fully scripted in Lua)
* Levels are scripted in Lua (planning for levels to be fully scripted in Lua)
* Gamemodes: Endless, Double-Trouble, Godmode
* Combos, multipliers, rounds, health
* Enemy explosions when sprites are available
* Statically compiled
* Default sprites if some or all are missing
* Skinning - just drop in a folder into the skins directory!
* Highscore system -- compete against your friends!

# Compiling
```bash
# ensure you have raylib installed, see below
make help

# build release
make release

# easiest
make play

# package all
make tar
make zip
```

# TODO
* differentiate highscores depending on game mode and starting ship
* Flower which spins
* Make more levels
* Flashlight mode
* Darker background option
* Make new `so_cb_strobe`
  * and a new mode called solar flare mode
* Level files can define skins and which background to use

# Requirements
* Raylib
  * Raylib 3.0.0 is used, however it seems 3.5.0 also works
  * Raylib 3.7.0 or higher does not work due to changes in the audio interface
* Linux
  * make, gcc, /usr/local/lib/libraylib.a
* Windows
  * git-bash, make, tcc/Raylib installation
* Mac
  * make, gcc/clang, /usr/local/lib/libraylib.a

# Raylib Installation
Go here to get the latest: [https://github.com/raysan5/raylib/releases](https://github.com/raysan5/raylib/releases)

## Linux
```bash
wget https://github.com/raysan5/raylib/releases/download/3.5.0/raylib-3.5.0_linux_amd64.tar.gz

tar xzf raylib-3.5.0_linux_amd64.tar.gz
cp raylib-3.5.0_linux_amd64/lib/libraylib.a /usr/local/lib/
```

## Windows
```bash
# make sure you have MinGW for Windows thru git-for-windows
https://git-scm.com/downloads

# Then go here
https://github.com/raysan5/raylib/releases

# download and run
 raylib_installer_v300.tcc.exe
```

## Mac
```bash
# Homebrew way, might get wrong version. Need 3.5.0
brew install raylib

# wget or just manually download it from github downloads page
wget https://github.com/raysan5/raylib/releases/download/3.5.0/raylib-3.5.0_macos.tar.gz

tar xzf raylib-3.5.0_macos.tar.gz
cp raylib-3.5.0_macos/lib/libraylib.a /usr/local/lib/
```

# Bugs
* Can't re-create: MENU from level select screen crashes
* Memory leak on module context switch, suspect there are resources overlooked
