# Beat Saber Linux Goodies

# Recommended Install Method
- Install Beat Saber through Steam
- Run Beat Saber once/Play a round
- Take latest Linux alpha from [My BeatDrop Fork](https://github.com/geefr/BeatDrop/releases) 
- Use BeatDrop to install BSIPA and other mods
- Run [The Fixup Script](https://github.com/geefr/beatsaber-linux-goodies/blob/master/scripts/bs-linux-modfix.sh) to patch the game - './bs-linux-modfix.sh &lt;Beat Saber Install directory (SteamApps/common/Beat Saber)&gt; &lt;Path to Proton Installation (SteamApps/common/Proton 3.7)&gt;'
- In steam right click Beat Saber -> Properties
- Check 'Force the use of a specific Steam Play compatibility tool
- Select the tool called 'Proton BeatSaber' (Might need to restart Steam)
- Go have fun

# What
Mod installation scripts and other goodies for running Beat Saber on Linux.

This repository is mainly for gathering information, and implementations of scripts/etc that aren't tied to any one tool.

The eventual target is to update existing modding tools to work on Linux directly, and allow 1-click installs for Beat Saber mods.

# Why
Beat Saber itself has run perfectly on Linux for quite some time, yet mod installation remains difficult.

There are some folks that want to use their Linux boxes for BS, however can't live without mods.

# How
- Research and gather information in this repo for the power users.
- Work on integrating the lower level scripts/etc into popular modding tools for folks that don't want to open the terminal or do things manually.

# Bugs/Support
If you're having issues on Linux it's probably best if you don't pester the BeatDrop/ModAssistant/Mod devs directly for now.
Raising a ticket here or on my BeatDrop fork is probably a better plan, and will keep the Linux-specific hacks in one place.
