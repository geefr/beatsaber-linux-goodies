#include "beatmodsv1.h"

#include <QThread>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

#include <QObject>

#include "settings.h"

BeatModsV1::BeatModsV1()
{

}

std::list<Mod> BeatModsV1::getMods( std::map<QString, QString> filters)
{

  // TODO: Hard coding approved here, maybe that should be on Settings
  QString urlStr = "https://beatmods.com/api/v1/mod?status=approved";
  for( auto& filter : filters )
  {
    urlStr = urlStr + "&" + filter.first + "=" + filter.second;
  }


  QUrl url(urlStr);
  std::unique_ptr<QNetworkReply> response (mNetMan.get(QNetworkRequest(url)));

  QEventLoop loop;
  QObject::connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if( response->error() != QNetworkReply::NoError )
  {
    qDebug() << "ERROR: Failed to fetch mod list from beatmods";
    return {};
  }

  QJsonDocument jsonDoc = QJsonDocument::fromJson(response->readAll());
  QJsonObject jsonObj = jsonDoc.object();
  QJsonArray jsonArr = jsonDoc.array();

  std::list<Mod> res;

  foreach( QJsonValue val, jsonArr )
  {
    QJsonObject obj = val.toObject();

    auto downloads = obj["downloads"].toObject();

    std::map<QString,QString> fileHashes;
    auto fileHashObj = downloads["hashMd5"];
    auto fileHashArr = fileHashObj.toArray();
    foreach( QJsonValue fileHash, fileHashArr ) {
      fileHashes[fileHash["file"].toString()] = fileHash["hash"].toString();
    }

    Download download = {
      .mType = downloads["type"].toString(),
      .mURL = downloads["url"].toString(),
      .mFileHashes = fileHashes,
    };

    Mod mod = {
      .mID = obj["_id"].toString(),
      .mDescription = obj["description"].toString(),
      .mGameVersion = obj["gameVersion"].toString(),
      .mName = obj["name"].toString(),
      .mDownload = download,
    };

    res.emplace_back(mod);
  }

  return res;
}

bool BeatModsV1::downloadMod( Mod mod, QFile& file )
{
  // Download a mod through the api and save it to the temporary dir
  // TODO: Hard coding approved here, maybe that should be on Settings
  QString urlStr = "https://beatmods.com/api/" + mod.mDownload.mURL;
  QUrl url(urlStr);
  std::unique_ptr<QNetworkReply> response (mNetMan.get(QNetworkRequest(url)));

  connect(response.get(), &QNetworkReply::readyRead, this, [&]() {
    file.write(response->readAll());
  });

  QEventLoop loop;
  connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if( response->error() != QNetworkReply::NoError )
  {
    qDebug() << "ERROR: Failed to fetch mod list from beatmods";
    return {};
  }

  return true;
}

