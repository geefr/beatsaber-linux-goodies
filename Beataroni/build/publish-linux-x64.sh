#!/usr/bin/env sh
buildDir=$(dirname $0)
dotnet publish /p:PublishProfile=${buildDir}/../Beataroni/Properties/PublishProfiles/Linux64.pubxml
cp ${buildDir}/Beataroni-Linux.sh ${buildDir}/../Beataroni/bin/Release/net6.0/publish-Linux64/
