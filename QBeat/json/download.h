#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QString>
#include <map>

struct Download
{
  QString mType;
  QString mURL;
  std::map<QString, QString> mFileHashes;
};

#endif // DOWNLOAD_H
