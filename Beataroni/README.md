# Beataroni
Beataroni is a graphical BeatSaber mod installer for Linux. In the future it may gain more features/platforms.

Beataroni is a spiritual successor to QBeat, but has some major differences:
* It's written in C#
* All dependencies are included in the release (It should run on your weird Linux distro)
* It doesn't require Wine or Winetricks for the installation (It's way faster)

Beataroni releases: [Github release page](https://github.com/geefr/beatsaber-linux-goodies/releases) 

# Mod Setup Walkthrough
Beataroni includes everything needed to setup mods on Linux, so no need to download the scripts, or anything else.

## Getting Beataroni
Head over to the [Release Page](https://github.com/geefr/beatsaber-linux-goodies/releases)  and grab the latest Beataroni release. Extract this to somewhere convenient.

The binary should work on any Linux system, with these quirks:
* On some systems it can be difficult to run the binary through a graphical file manager. Execute Beataroni-Linux.sh instead and it should launch the UI.

## First Time Setup
These steps should only need to be run once on your machine.
If there's a beat saber update you'll need to re-run Beataroni.

**Make sure you have run Beat Saber once before installing any mods**

Install steps:
* Run Beataroni
* Paste the path to your beatsaber install into the first page
* Double check the game version before continuing (Defaults to latest beat saber version)
* Click continue
* Select which mods you want on your system
* Click continue
* Wait for installation to complete
* Read the contents of the panel, check that there's no errors
* If there are errors copy the log and either raise a ticket here, or ping ITGaz on the BSMG discord.
* If installation succeeds close Beataroni, and play the game

## Adding More Mods
* Re-run Beataroni
* Change your mod selection
* Complete the install process

# Limitations/Known Bugs
* Beataroni is new code, there will be bugs
* Only Steam installations are supported (If you're somehow using oculus on Linux I'll happily add support, wouldn't be difficult)
* The install package of Beataroni is a bit weird, as the binary is actually a shared library (Thanks Microsoft). Beataroni-Linux.sh will launch it correctly, or just ./Beataroni from a terminal window until i can work out a better solution.

# Thanks
Beataroni includes a copy of [BSIPA](https://github.com/bsmg/BeatSaber-IPA-Reloaded), which has been forked/minimised in order to create a Linux-native executable. See [BSIPA Linux](https://github.com/geefr/BSIPA-Linux).

BSIPA is Copyright (c) 2019 Anairkoen Schno, distributed under the MIT licence.

Beataroni logo/icon by Ticia Schiffner (CC0 licence).

General thanks go to any early testers, all the mod developers, and the wider community. Stay awesome.

