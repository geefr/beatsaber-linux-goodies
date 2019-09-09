#ifndef MOD_H
#define MOD_H

#include <QString>
#include <list>

#include "download.h"

struct Mod
{
  QString mID;
  QString mDescription;
  QString mGameVersion;
  QString mName;
  QString mVersion;
  std::list<Download> mDownloads;
  std::list<Mod> mDependencies;
};

#endif
