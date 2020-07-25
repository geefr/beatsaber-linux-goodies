#!/usr/bin/env bash
#set -euo pipefail

# Copyright (c) 2020, Gareth Francis (gfrancis.dev@gmail.com)
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

set -x

compatTools="${HOME}/.steam/root/compatibilitytools.d/"
compatData="${bsInstall}/../../compatdata/620980"

cat << EOF
This script will setup beat saber mods on your system
Before running make sure the following have been done:
 - Wine is installed on your system
 - Your wine installation supports .Net 4.6.1 or higher
 - Beat Saber has been installed through steam
 - Beat Saber has been run/played at least once
 - That the BSIPA mod has been installed
   - Can be installed manually, or using a mod installer such as QBeat, ModAssistant, or BeatDrop
   - If the mod installer failed to run IPA.exe don't worry, this script will perform the patching step

To run this script you need to pass some arguments:
 - The installation folder of beat saber itself e.g SteamApps/common/Beat Saber
 - (Optional) Wine prefix - Script will use WINEPREFIX or ~/.wine if not provided
EOF

if [ $# -lt 1 ]; then
  echo "USAGE: ./bs-linux-modfix.sh <Beat Saber Install directory> [Wine Prefix (Optional)]"
  exit 1
fi

if [ $# -ne 2 ]; then
  winePrefix=${WINEPREFIX}
  if [ ! -d ${winePrefix} ]; then
    winePrefix="$HOME/.wine"
  fi
else
  winePrefix=${2}
fi

if ! ./bs-linux-is-wine-valid.sh ${winePrefix} > /dev/null; then
  echo "ERROR: Your wine installation doesn't appear to be valid, please ensure you have wine installed, and .Net 4.6.1 is installed in \$WINEPREFIX"
  exit 1
fi

# Patching BS with IPA.exe
bsInstall=$(realpath "${1}")
pushd "${bsInstall}" &> /dev/null

# TODO: Would be nice to exploit the Proton installation here, or otherwise not require the user to deal with winetricks
#WINEPATH="${bsProtonDir}/dist/bin/wine64" WINEPREFIX="${bsProtonDir}/dist/share/default_pfx" "${bsProtonDir}/dist/bin/wine64" IPA.exe
# TODO: Would be nice to be able to detect if .net 4.6.1 is supported by wine and quit otherwise
# For now system wine must be setup with at least dotnet461 installed
if ! WINEPREFIX=${winePrefix} wine IPA.exe -n 2> /dev/null; then
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
echo "SUCCESS: Beat Saber has been modded successfully, have fun hitting blocks <3"

popd &> /dev/null
