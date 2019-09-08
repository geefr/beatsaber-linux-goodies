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
  std::list<Download> mDownloads;
};

#endif
