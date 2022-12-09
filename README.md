# Beat Saber Linux Goodies
Mod installation scripts and other goodies for running Beat Saber on Linux.

This repository is mainly for gathering information, and implementations of tools to make setup easier.

# Bugs/Support
If you're having issues on Linux the modding discord is fairly friendly, or raise a ticket here if it's an issue with my tools.

Please don't annoy the ModAssistant/mod devs directly, or at least be nice and don't expect full Linux support on everything.

# Install Method: BeatSaberModManager

Introduced in mid-2022, BeatSaberModManager seems to have taken up the torch for Linux support (also Windows, and a bunch of features on top of just installing mods).

If you find yourself in the BSMG Discord posting messages like "beataroni doesn't do X and I hate it" or "I think beataroni has a bug", while also not sending me any bug reports, you should be using BeatSaberModManager instead ;)

I don't know much about it myself, but best of luck to this one. I'm happy for any of my linux-goodies to be pulled in or used as reference, following the spirit of the licence.

BetaSaberModManager: [Github Page](https://github.com/affederaffe/BeatSaberModManager)

# Install Method: Beataroni

December 2022: Beataroni is in feature-freeze:
* Bugs relating to basic functionality of mod installation will be fixed
* Bugs will only be fixed if they are raised here (and not buried several channels deep in the Discord servers)
* New features will not be added, you're free to fork though

Beataroni is an a GUI mod installer for Linux. In the future it may gain more features/platforms.

Beataroni is a spiritual successor to QBeat, but has some major differences:
* It's written in C#
* All dependencies are included in the release (It should run on your weird Linux distro)
* It doesn't require Wine or Winetricks for the installation (It's way faster)

Beataroni releases: [Github release page](https://github.com/geefr/beatsaber-linux-goodies/releases) 
Beataroni Docs:  [See Here, scroll down](https://github.com/geefr/beatsaber-linux-goodies/tree/live/Beataroni)

# Install Method: QBeat

December 2020: QBeat was an older mod installer, it was deprecated in favour of Beataroni. Code and information is preserved in case it's useful for anyone.

QBeat can be used to setup wine/proton, download mods, and patch beat saber in a relatively easy manner.

QBeat releases: [Github release page](https://github.com/geefr/beatsaber-linux-goodies/releases) 
QBeat Docs:  [See Here, scroll down](https://github.com/geefr/beatsaber-linux-goodies/tree/live/QBeat)

# Install Method: Mod Installer + fix scripts
Other mod installers can be used for mod isntallation, you'll just need to run scripts/bs-linux-modfix.sh after mod installation.

If you use this approach I assume you know your way around Linux/Wine, and will provide minimal support.

See the relevant folders here for a quick readme. Mod Assistant would be the best option here, should work fine as long as your wine install has .Net support.

