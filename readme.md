![Logo](https://github.com/Eli5723/StakkenRefactor/blob/master/release/resources/textures/logo.png?raw=true)
# Stakken: A Cultris 2 Spirtitual Successor

Stakken is a spiritual successor to Cultris 2, a fast-paced multiplayer Tetris game that emphasizes speed and technique over arbitrary garbage systems.    
Stakken aims to improve upon the Cultris 2 experience by improving in the following areas: 
- Cross platform support
- Improved Performance
- Greater Customizability (more options for controllers, gamemodes, visuals, and sounds)
- Greater Accessiblity / Localization

## Building on Windows
Not currently supported

## Building on Linux
Bootstrap vcpkg
```
git submodule update --init --recursive
./boostrap-vcpkg.sh
mkdir build
cmake --build       
```
The game searches for assets in a path relative to the executable, so we symlink the Resources folder into the output directory, ``/bin``
```
ln -s -r ./Resources ./bin/Resources
```
