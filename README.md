# Beat Saber Linux Goodies

# Recommended Install Method

At the moment there's no fully integrated solution, but if you know how to run a shell script you'll be fine ;)

For downloading mods the recommended method is [My BeatDrop Fork](https://github.com/geefr/BeatDrop/releases). This is a native Linux port of BeatDrop with a few tweaks here and there.

An alternative for mod downloads is QBeat, currently a very early prototype. See [Here](https://github.com/geefr/beatsaber-linux-goodies/tree/QBeat/QBeat) if you want to try it but I make no promise that it'll work properly.

In addition to mod downloads you'll need to ensure your system's wine installation is up to scratch, as a default install can't run BSIPA.

See the [scripts directory](scripts/README.md) for instructions/further detail.

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
