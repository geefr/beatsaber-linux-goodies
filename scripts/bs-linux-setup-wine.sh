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

#set -x

# This script sets up a wine prefix such that it can run BSIPA
# That means installing:
# - dotnet461 through winetricks
#
# Variables:
# - WINETRICKSURL, optional
# Arguments:
# - wine prefix, required
# Returns:
# 0 if installation was successful

if [ $# -ne 1 ]; then
  echo "USAGE: ${0} <Wine Prefix> : Sets up a wine prefix for running BSIPA"
  exit 1
fi

winePrefix=$(realpath ${1})
prefixExists=0

if ! command -v wine > /dev/null; then
  echo "ERROR: Wine doesn't appear to be installed on your system, please do so and ensure it's in your PATH"
  exit 1
fi

if ! command -v cabextract > /dev/null; then
  echo "ERROR: cabextract is required to install dotnet 4.6.1, please ensure it's in your PATH"
  exit 1
fi

function prefixwarn {
  if [ $prefixExists -eq 1 ]; then
    echo "WARN: A Wine prefix already exists at ${winePrefix}. If you experience any issues, please delete the prefix, or use a different prefix."
  fi
}

if [ -d "${winePrefix}" ]; then
  prefixExists=1
fi

prefixwarn

mkdir -p ${winePrefix} 2> /dev/null
pushd ${winePrefix} > /dev/null
if [ -z $WINETRICKSURL ]; then
  WINETRICKSURL=https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks
fi
if ! wget $WINETRICKSURL -O winetricks 2> /dev/null; then
  echo "ERROR: Failed to download winetricks, please log this as a bug at https://github.com/geefr/beatsaber-linux-goodies"
  exit 1
fi
chmod +x winetricks
popd > /dev/null

wineVersion=`wine --version | cut -d ' ' -f1 | sed -e 's/wine-//' -e 's/-rc.*//'` # Get Wine version

workaround49532=0
if [[ $( bc<<<"${wineVersion}>=5.12" ) -eq 1 ]] && [[ $( bc<<<"${wineVersion}<5.18" ) -eq 1 ]]; then
  echo "WARN: Wine version is wine-${wineVersion}; working around wine bug 49532."
  workaround49532=1
  ./workarounds/wine-49532-workaround.sh & # Execute workaround script as background process
  workaround49532pid=$!
fi

workaround49897=0
if [[ $( bc<<<"${wineVersion}>=5.18" ) -eq 1 ]]; then
  echo "WARN: Wine version is wine-${wineVersion}; working around wine bug 49897."
  workaround49897=1
  ./workarounds/wine-49897-workaround.sh & # Execute workaround script as background process
  workaround49897pid=$!
fi

if ! WINEPREFIX=${winePrefix} ${winePrefix}/winetricks -f -q dotnet461 2> /dev/null; then
  echo "ERROR: Failed to install .Net 4.6.1"
  if [ $workaround49532 -eq 1 ]; then
    kill "$workaround49532pid"
    pkill mscorsvw.exe # In case we missed any
  fi

  if [ $workaround49897 -eq 1 ]; then
    kill "$workaround49897pid"
    pkill mscorsvw.exe # In case we missed any
  fi
  prefixwarn
  exit 1
fi

if [ $workaround49532 -eq 1 ]; then
  kill "$workaround49532pid"
  pkill mscorsvw.exe # In case we missed any
fi
if [ $workaround49897 -eq 1 ]; then
  kill "$workaround49897pid"
  pkill mscorsvw.exe # In case we missed any
fi

if ! ./bs-linux-is-wine-valid.sh ${winePrefix} &> /dev/null; then
  echo "ERROR: .Net installation succeeded but wine prefix doesn't appear valid"
  prefixwarn
  exit 1
fi

prefixwarn
echo "SUCCESS: Wine prefix at ${winePrefix} setup to run BSIPA"
