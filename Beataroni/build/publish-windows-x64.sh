#!/usr/bin/env sh
buildDir=$(dirname $0)
dotnet publish /p:PublishProfile=${buildDir}/../Beataroni/Properties/PublishProfiles/Win64.pubxml
