# IPA
[IPA](https://github.com/beat-saber-modding-group/BeatSaber-IPA-Reloaded) is the core magic sauce that allows mod hooking in beatsaber:

# Running through Wine
IPA.exe can be run under wine, just make sure you've got .Net 4.6.1 installed
Simplest option for this is 'winetricks dotnet461'

# Running through Beat Saber's Proton install
In theory this is possible, currently investigating as the best option
Should 'just' need a copy of the .net runtime to be in the path, then any version of wine should pick it up
- This is easier said than done, if you can help please get in touch
- There may be licence issues with distributing Microsoft's .net libs, haven't checked yet

# Running without winetricks dotnet461
This would be very useful for automated Linux installs, as the user wouldn't need to install anything before running the script ;)

## Mono
Not possible (yet)
- IPA makes calls into windows system libraries with P/Invoke
- Might be possible to build IPA.exe under mono, haven't tried yet


## .Net Core
Again might be possible to build IPA under .net core, which would allow a Linux native executable



