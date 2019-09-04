# Mod Assistant
If you use Beat Saber mods I'm sure you're aware of Mod Assistant

Current Status: No work planned yet, need to discuss with maintainers for best way forward, choice is between running under wine or providing a native Linux build.

# Running through Wine
I haven't had much success on this front, but believe it's possible.
As a minimum you'll need a copy of wine with .Net 4.6.1 or higher to run mod assistant, you should be able to use [WineTricks](https://wiki.winehq.org/Winetricks) to set things up.

Note that the Proton installation provided by Steam does not include the required libraries

# Native Linux/.Net Core Port
I spent a little time trying to port modassistant to .Net Core to obtain a Linux native version.
While most of the code built out of the box the choice of GUI library means an in-depth rewrite would be needed to support .Net Core.
