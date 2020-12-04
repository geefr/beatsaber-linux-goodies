#!/usr/bin/env sh
dotnet publish /p:PublishProfile=../Beataroni/Properties/PublishProfiles/Linux64.pubxml
cp Beataroni-Linux.sh ../Beataroni/bin/Release/net5.0/publish-Linux64/

