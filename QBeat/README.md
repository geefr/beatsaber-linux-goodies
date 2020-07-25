# QBeat
A Cross platform mod installer for Beat Saber

Current state is fairly primitive, but does provide an alternative to BeatDrop/ModAssistant on Linux
- Limited functionality - Actions are fairly dumb right now
- Not heavily tested, only test on Linux (/should/ build and run on windows, might break things)
- Just a command line interface at first, gui will come once base actions are implemented

If you're playing beat saber on Linux let me know how it goes, feel free to raise feature requests/bugs through github.

Like all early software this might break your beatsaber installation, delete your files, and make the kittens cry.
If you're unfamiliar with Linux in general or don't know what you're doing please ask first, or wait until the repo hits version 1.0 ;)

# Mod Setup Walkthrough
QBeat includes everything needed to setup mods on Linux, so no need to download the scripts separately. 

## Getting QBeat
Head over to the [Release Page](https://github.com/geefr/beatsaber-linux-goodies/releases)  and grab the latest QBeat tarball. Extract this to somewhere convenient.

These binaries should work on most systems, will be built against Qt 5.9 or thereabouts.

## First Time Setup
These steps should only need to be run once on your machine.
If there's a beat saber update you'll need to re-install mods/patch again but the config and wine setup should only need to be done once.

**Make sure you have run Beat Saber once before running any of these commands**

These commands need to be run within a terminal, and assume QBeat is in the current working directory

```
# If QBeat has never been run you'll need to setup the paths.
./QBeat --config set winePrefix "~/.wine"
./QBeat --config set bsInstall "~/.steam/steam/steamapps/common/Beat Saber"

# QBeat doesn't detect the beat saber version, so update if needed
# For the version here it's usually the current game version, but if mods don't need updating then
# beatmods just lists them under the old version.
# Best way to be sure here is check what version ModAssistant installs and match that
./QBeat --config set gameVersion "1.8.0"

# Validate that your wine installation is correct
./QBeat --validate-wine
# If this fails then there's a command to set things up. This one may take a while so please be patient
./QBeat --setup-wine

# Install the mandatory/base mods
./QBeat --install "BSIPA"

# Run BSIPA to patch the game (Remember, you need to have run BS once before doing this)
# Once the game has been patched you don't need to re-run this, at least until there's an update
./QBeat --patch


# List available mods, and install others as you like
./QBeat --list
./QBeat --install "ScoreSaber"
./QBeat --install "Custom Avatars"

# Or just install everything like some crazy person. If you do this you might have performance issues in game.
./QBeat --install-all

```

With any luck you should have a modded copy of Beat Saber now

## Adding More Mods
```
# Once the game is patched you can just pile more mods into the installation
./QBeat --install "Perfection Display"
./QBeat --install "HTTP Status"
```

# Features
- Installation of mods on Linux, package manager style interface
- Ability to setup wine prefix for running BSIPA
- Ability to apply the other Linux-specific tweaks here
- Ability to patch game with BSIPA
- Config values allow setting steam v oculus, game version, and other parameters. Run QBeat --config get to list variables

# Limitations
- QBeat --install : Will install latest version of a mod and it's dependencies, no checking of current version, will overwrite if already present
- There's no way to list currently installed mods
- BSIPA will not be run automatically
- No gui (yet)

# Workarounds
There's a few environment variables that set workarounds. I recommend only setting these if you understand the consequences.
Workaround will activate if these are set, value doesn't matter.
- QBEAT_DISABLE_SSLVERIFY : Disable SSL verification, obvious security warning here
- QBEAT_ALLOW_ANY_SSL : Allow any SSL version

## Building QBeat

```
# Ensure your system has cmake, gcc, minizip, and Qt5 development libraries
sudo apt install gcc g++ cmake libQt5*-dev libminizip-dev git

# Grab the source
git clone https://github.com/geefr/beatsaber-linux-goodies
cd beatsaber-linux-goodies/QBeat

# Configure and build QBeat. By default the install prefix will be /usr/local, so you might want to change it
# If any dependencies are missing you should get a fairly obvious error
cmake -DCMAKE_INSTALL_PREFIX=<installation prefix>
make install
```
