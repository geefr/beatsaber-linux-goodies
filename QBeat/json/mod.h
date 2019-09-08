#ifndef MOD_H
#define MOD_H

#include <QString>
#include "download.h"

struct Mod
{
  QString mID;
  QString mDescription;
  QString mGameVersion;
  QString mName;
  Download mDownload;
};

#endif
