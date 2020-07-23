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

# This script validates whether the wine installation on the system is valid for use with BSIPA
# In order to be considered valid:
# - The wine installation requires .net 4.6.1 installed
# - TODO: This script may not be perfect. It may return true for other similar versions of .net
#
# Arguments: None
# Environment: WINEPREFIX should be set to the prefix to check
# Returns: 0 if the prefix seems valid

if [ $# -ne 1 ]; then
  echo "USAGE: ${0} <Wine Prefix>: Validates whether a wine prefix is setup for running BSIPA"
  exit 1
fi

if ! command -v wine > /dev/null; then
  echo "ERROR: Wine doesn't appear to be installed on your system, please do so and ensure it's in your PATH"
  exit 1
fi

winePrefix=$(realpath ${1})

if [ ! -d "${winePrefix}" ]; then
  echo "ERROR: Wine prefix at ${winePrefix} doesn't exist"
  exit 1
fi

if [ ! -f "${winePrefix}/drive_c/windows/Microsoft.NET/Framework/v4.0.30319/Microsoft.CSharp.dll" ]; then
  echo "ERROR: Wine prefix at ${winePrefix} doesn't contain the expected .Net installation"
  exit 1
fi

echo "SUCCESS: Wine prefix at ${winePrefix} should be able to run BSIPA"
