#!/usr/bin/env bash

# Copyright (c) 2019, Gareth Francis (gfrancis.dev@gmail.com)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#set -x

bsProtonName="Proton BeatSaber"
compatTools="${HOME}/.steam/root/compatibilitytools.d/"
bsProtonDir="${compatTools}/${bsProtonName}"
compatData="${bsInstall}/../../compatdata/620980"

echo "This script will setup beat saber mods on your system"
echo "Before running make sure the following have been done:"
echo " - Wine is installed on your system"
echo " - Your wine installation supports .Net 4.6.1 or higher"
echo " - Beat Saber has been installed through steam"
echo " - Beat Saber has been run/played at least once"
echo " - BeatDrop or ModAssistant has been run, and the BSIPA mod has been installed (Don't worry if IPA.exe didn't patch correctly, we'll handle that here"
echo " - Ideally that the copy of proton passed to this script has not been modified, or if so has been reinstalled through steam first (Feel free to try with a dirty copy, might be bugs in that case)"
echo ""
echo "To run this script you need to pass 2 arguments:"
echo " - The installation folder of beat saber itself e.g SteamApps/common/Beat Saber"
echo " - The installation folder of the Proton version you want to use e.g SteamApps/common/Proton 4.11"
echo ""
echo "The Proton installation will be copied to ~/.steam/root/compatibilitytools.d/Proton BeatSaber"
echo "TODO: To workaround https://github.com/beat-saber-modding-group/BeatSaber-IPA-Reloaded/issues/18 this script will modify SteamApps/compatdata/620980. If something doesn't work right just delete this folder and validate your beat saber installation"
echo ""
echo "After running this script you will need to"
echo " - Restart Steam"
echo " - Change the proton version user for beat saber to 'Proton BeatSaber'"
echo " - Go have fun <3"

if [ $# -lt 2 ]; then
  echo "USAGE: ./bs-linux-modfix.sh <Beat Saber Install directory> <Beat Saber Proton Installation> [Wine Prefix (Optional)]"
	exit 1
fi

if [ $# -ne 3 ]; then
  winePrefix=${WINEPREFIX}
  if [ ! -d ${winePrefix} ]; then
    winePrefix="$HOME/.wine"
  fi
else
  winePrefix=${3}
fi

if ! ./bs-linux-is-wine-valid.sh ${winePrefix} > /dev/null; then
  echo "ERROR: Your wine installation doesn't appear to be valid, please ensure you have wine installed, and .Net 4.6.1 is installed in \$WINEPREFIX"
  exit 1
fi

bsInstall=$(realpath "${1}")
protonInstall=$(realpath "${2}")

echo "Creating custom Proton installation for Beat Saber use"
rm -rf "${bsProtonDir}" || true
mkdir -p "${compatTools}"
cp -r "${protonInstall}" "${bsProtonDir}"

if [ $? -ne 0 ]; then
	echo "Failed to copy Proton installation"
	exit 1
fi

# Setup the tool config for steam
cat <<EOM >"${bsProtonDir}/compatibilitytool.vdf"
"compatibilitytools"
{
  "compat_tools"
  {
    "${bsProtonName}"
    {
      "install_path" "."
      "display_name" "${bsProtonName}"
      "from_oslist"  "windows"
      "to_oslist"    "linux"
    }
  }
}
EOM

mv "${bsProtonDir}/dist/lib64/wine/winhttp.dll.so" "${bsProtonDir}/dist/lib64/wine/winhttp_alt.dll.so" &> /dev/null || true
mv "${compatData}/pfx/drive_c/windows/syswow64/winhttp.dll" "${compatData}/pfx/drive_c/windows/syswow64/winhttp_alt.dll" &> /dev/null || true

# Patching BS with IPA.exe
pushd "${bsInstall}" &> /dev/null

# TODO: Would be nice to exploit the Proton installation here, or otherwise not require the user to deal with winetricks
#WINEPATH="${bsProtonDir}/dist/bin/wine64" WINEPREFIX="${bsProtonDir}/dist/share/default_pfx" "${bsProtonDir}/dist/bin/wine64" IPA.exe
# TODO: Would be nice to be able to detect if .net 4.6.1 is supported by wine and quit otherwise
# For now system wine must be setup with at least dotnet461 installed
WINEPREFIX=${winePrefix} wine IPA.exe -n 2> /dev/null

if [ $? -ne 0 ]; then
	echo "WARNING: IPA.exe returned non-zero result"
fi

# Configure wine registry to ensure winhttp.dll loads correctly
userRegFile="${bsInstall}/../../compatdata/620980/pfx/user.reg"
echo '[Software\\Wine\\DllOverrides]' >> "${userRegFile}"
echo '"winhttp"="native,builtin"' >> "${userRegFile}"

if [ $? -ne 0 ]; then
        echo "ERROR: Failed to add Wine DllOverrides to ${userRegFile}"
	exit 1
fi

echo ""
echo "SUCCESS: Beat Saber has been modded successfully, have fun hitting block <3"

popd &> /dev/null

