#ifndef BEATMODSV1_H
#define BEATMODSV1_H

#include <QNetworkAccessManager>

#include "mods/mod.h"

class BeatModsV1
{
public:
  BeatModsV1();
  std::list<Mod> getMods(std::map<QString, QString> filters = {});
private:
  QNetworkAccessManager mNetMan;
};

#endif
