# MG: MyGalaga
Space shooter, galaga-like game. Note that I don't own these assets, and they will not be available for download.

```bash
git clone git@github.com:/JacobLondon/mg --recurse-submodules
```

![](https://user-images.githubusercontent.com/17059471/126079271-e560e05b-bd8f-429b-96b5-016c79d9e1de.png)

# Features
* Xbox Controller Support
* Backgrounds are simple to create and customize using the scene_man.c library in atmos.c module. (Planning for backgrounds to be fully scripted in Lua)
* Levels are scripted in Lua (planning for levels to be fully scripted in Lua)
* Gamemodes: Endless, Double-Trouble, Godmode
* Combos, multipliers, rounds, health
* Enemy explosions when sprites are available
* Statically compiled
* Default sprites if some or all are missing

# Compiling
```bash
# ensure you have raylib installed, see below
make help
make release
make tar
make zip
```

# TODO
* Flower which spins
* Make more levels
* Powerups and drops
* Level select with high score stored in level file
* Flashlight mode
* Darker background option
* Make new `so_cb_strobe`
  * and a new mode called solar flare mode
* Level files can define assets and which background to use

# Requirements
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
