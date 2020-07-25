#!/usr/bin/env bash
#set -euo pipefail

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

# This script sets up a wine prefix such that it can run BSIPA
# That means installing:
# - dotnet461 through winetricks
#
# Arguments: None
# Environment: WINEPREFIX should be set to the prefix to check
# Returns: 0 if installation was successful

if [ $# -ne 1 ]; then
  echo "USAGE: ${0} <Wine Prefix> : Sets up a wine prefix for running BSIPA"
  exit 1
fi

winePrefix=$(realpath ${1})

if ! command -v wine > /dev/null; then
  echo "ERROR: Wine doesn't appear to be installed on your system, please do so and ensure it's in your PATH"
  exit 1
fi

if ! command -v cabextract > /dev/null; then
  echo "ERROR: cabextract is required to install dotnet 4.6.1, please ensure it's in your PATH"
  exit 1
fi

mkdir -p ${winePrefix} 2> /dev/null
pushd ${winePrefix} > /dev/null
if ! wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks 2> /dev/null; then
  echo "ERROR: Failed to download winetricks, please log this as a bug at https://github.com/geefr/beatsaber-linux-goodies"
  exit 1
fi
chmod +x winetricks
popd > /dev/null

if ! WINEPREFIX=${winePrefix} ${winePrefix}/winetricks dotnet461 2> /dev/null; then
  echo "ERROR: Failed to install .Net 4.6.1"
  exit 1
fi

if ! ./bs-linux-is-wine-valid.sh ${winePrefix} &> /dev/null; then
  echo "ERROR: .Net installation succeeded but wine prefix doesn't appear valid"
  exit 1
fi

echo "SUCCESS: Wine prefix at ${winePrefix} setup to run BSIPA"
