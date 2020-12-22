# Beat Saber Linux Goodies
Mod installation scripts and other goodies for running Beat Saber on Linux.

This repository is mainly for gathering information, and implementations of tools to make setup easier.

# Bugs/Support
If you're having issues on Linux the modding discord is fairly friendly, or raise a ticket here if it's an issue with my tools.

Please don't annoy the ModAssistant/mod devs directly, or at least be nice and don't expect full Linux support on everything.

# Install Method: Beataroni
Beataroni is an a GUI mod installer for Linux. In the future it may gain more features/platforms.

Beataroni is a spiritual successor to QBeat, but has some major differences:
* It's written in C#
* All dependencies are included in the release (It should run on your weird Linux distro)
* It doesn't require Wine or Winetricks for the installation (It's way faster)

Beataroni releases: [Github release page](https://github.com/geefr/beatsaber-linux-goodies/releases) 
Beataroni Docs:  [See Here, scroll down](https://github.com/geefr/beatsaber-linux-goodies/tree/live/Beataroni)

# Install Method: QBeat
Note: As of December 2020 QBeat is deprecated, and Beataroni should be used instead (QBeat may still get bugfixes if you ask nicely, but will have not undergo active development).

QBeat can be used to setup wine/proton, download mods, and patch beat saber in a relatively easy manner.

QBeat releases: [Github release page](https://github.com/geefr/beatsaber-linux-goodies/releases) 
QBeat Docs:  [See Here, scroll down](https://github.com/geefr/beatsaber-linux-goodies/tree/live/QBeat)

# Install Method: Mod Installer + fix scripts
Other mod installers can be used for mod isntallation, you'll just need to run scripts/bs-linux-modfix.sh after mod installation.

If you use this approach I assume you know your way around Linux/Wine, and will provide minimal support.

See the relevant folders here for a quick readme. Mod Assistant would be the best option here, should work fine as long as your wine install has .Net support.

