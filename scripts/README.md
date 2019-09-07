# BeatSaber Linux Scripts

## Wine Scripts
These scripts checks to see if your wine prefix is setup for running BSIPA, and if not will install what you need.
```
# Set the location of your wine prefix
# If not set the default of ~/.wine will be used
export WINEPREFIX=~/.wine/wineprefix=beatsaber-linux-goodies-ipa

# Check whether wine is setup correctly
./bs-linux-is-wine-valid.sh

# If this script returns non-zero, or shows an error you'll need to install 
# For this just run the other script, and follow the prompts when the various setup wizards appear
./bs-linux-setup-wine.sh

# This script will double-check if the prefix is valid at the end, if it doesn't work please raise a github issue
```

## Beat Saber Mod Fix
This script needs to be run after you've downloaded mods through BeatDrop/similar.
(Work to integrate this into BeatDrop is in progress, these scripts will remain if you prefer using the terminal)

The main actions it performs are to:
- Run BSIPA to patch the game
- Apply various tweaks to Proton and the game's prefix so things work correctly
```
# Before running this script ensure you have:
# - Beat saber installed
# - Beat saber run at least once
# - Mods/BSIPA downloaded
#
# First ensure your wine prefix is set correctly
export WINEPREFIX=~/.wine/wineprefix=beatsaber-linux-goodies-ipa

# Run the script
# The proton installation here will be copied and modified to apply some fixes
./bs-linux-modfix.sh ~/.steam/steam/steamapps/common/Beat\ Saber ~/.steam/steam/steamapps/common/Proton\ 3.7
```

With any luck you should have a modded copy of Beat Saber now, only one this left to do
- Restart Steam
- Right click Beat Saber -> Properties
- Check 'Force the use of a specific Steam Play compatibility tool
- Select the tool called 'Proton BeatSaber'
- Go have fun
