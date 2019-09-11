# Beat Saber Linux Goodies

# Install Method 1: Beatdrop + script hacks
At the moment there's no fully integrated solution, but if you know how to run a shell script you'll be fine ;)

For downloading mods probably the simplest method is [My BeatDrop Fork](https://github.com/geefr/BeatDrop/releases). This is a Linux port of BeatDrop with a few tweaks here and there.

In addition to mod downloads you'll need to ensure your system's wine installation is up to scratch, as a default install can't run BSIPA.

See the [scripts directory](scripts/README.md) for instructions/further detail.

# Install Method 2: QBeat
An alternative for mod downloads is QBeat. This is currently under development but is functional enough to install get things running.

The advantage of QBeat is that it includes everything needed for the setup in one tool; It can be used to setup your wine/proton installations, download mods, and patch beat saber without needing to directly use the scripts.

See [Here](https://github.com/geefr/beatsaber-linux-goodies/tree/QBeat/QBeat) if you want to try it, I'd greatly appreciate feedback on this so get in touch if there's any questions.


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
