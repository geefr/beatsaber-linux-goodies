#ifndef BEATMODSV1_H
#define BEATMODSV1_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTemporaryDir>

#include "json/mod.h"
#include "json/download.h"

class BeatModsV1 : public QObject
{
  Q_OBJECT
public:
  BeatModsV1();
  /**
   * Fetch a list of mods from the api
   * @filters A list of filters to apply to apply in the request in the form <param,value>
   */
  std::list<Mod> getMods(std::map<QString, QString> filters = {});

  /**
   * Download a mod file and output the data to file
   * @param file An open file for data to be saved into
   */
  bool downloadModFile( Download mod, QFile& file);

private:
  void processModJson( std::list<Mod>& res, QJsonObject obj );

  QNetworkAccessManager mNetMan;
};

#endif
