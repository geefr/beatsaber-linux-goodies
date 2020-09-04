# BeatSaber Linux Scripts

## Wine Scripts
These scripts checks to see if your wine prefix is setup for running BSIPA, and if not will install what you need.
```
# Check whether wine is setup correctly
# If the wine prefix isn't passed scripts will use WINEPREFIX, or ~/.wine
./bs-linux-is-wine-valid.sh ~/.wine

# If this script returns non-zero, or shows an error your wine won't be able to patch beat saber
# For this just run the other script, and follow the prompts when the various setup wizards appear
./bs-linux-setup-wine.sh

# This script will double-check if the prefix is valid at the end, if it doesn't work please raise a github issue
```
**NOTE: An issue ([#1596](https://github.com/Winetricks/winetricks/issues/1596)) with winetricks, known since the 26th of August 2020 has made dotnet4xx installation fail. Until PR [#1599](https://github.com/Winetricks/winetricks/pull/1599) is merged, set the `WINETRICKSURL` environment variable to `https://raw.githubusercontent.com/SwimmingTiger/winetricks/master/src/winetricks` when running `./bs-linux-setup-wine.sh`.**

## Beat Saber Mod Fix
This script needs to be run after you've downloaded mods (BSIPA) through a mod installer

The main actions it performs are to:
- Run BSIPA to patch the game
- Apply various tweaks to Proton and the game's prefix so things work correctly
```
# Before running this script ensure you have:
# - Beat saber installed
# - Beat saber run at least once
# - Mods/BSIPA downloaded
#
# Run the script
./bs-linux-modfix.sh ~/.steam/steam/steamapps/common/Beat\ Saber ~/.wine
```

With any luck you should have a modded copy of Beat Saber now, just start the game and play.
