#!/usr/bin/env sh
buildDir=$(dirname $0)
dotnet publish /p:PublishProfile=${buildDir}/../Beataroni/Properties/PublishProfiles/Linux64.pubxml
cp ${buildDir}/Beataroni-Linux.sh ${buildDir}/../Beataroni/bin/Release/net5.0/publish-Linux64/
cp ${buildDir}/../../IPA/bin-minimal-linux64/IPA-Minimal ${buildDir}/../Beataroni/bin/Release/net5.0/publish-Linux64/

