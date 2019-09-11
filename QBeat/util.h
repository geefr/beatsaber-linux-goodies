#ifndef UTIL_H
#define UTIL_H

#include <QString>

class Util
{
public:
  static bool extractArchive( QString archivePath, QString destDirectory );
};

#endif
