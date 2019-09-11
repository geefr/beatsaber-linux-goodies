#include "beatmodsv1.h"

#include <QThread>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

#include <QObject>

#include <memory>

#include "settings.h"

BeatModsV1::BeatModsV1()
{

}

std::list<Mod> BeatModsV1::getMods( std::map<QString, QString> filters)
{
  QTextStream qOut( stdout );
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
    qOut << "ERROR: Failed to fetch mod list from beatmods\n";
    return {};
  }

  QJsonDocument jsonDoc = QJsonDocument::fromJson(response->readAll());
  QJsonObject jsonObj = jsonDoc.object();
  QJsonArray jsonArr = jsonDoc.array();

  std::list<Mod> res;


  foreach( QJsonValue val, jsonArr )
  {
    QJsonObject obj = val.toObject();
    processModJson( res, obj );
  }

  return res;
}

void BeatModsV1::processModJson( std::list<Mod>& res, QJsonObject obj )
{
  auto downloads = obj["downloads"].toArray();
  std::list<Download> resDownloads;

  foreach( QJsonValue download, downloads)
  {
    std::map<QString,QString> fileHashes;
    QString typeStr;
    QString urlStr;

    auto downloadObj = download.toObject();

    // TODO: This bit about downloads is a total mess, sort it out
    urlStr = downloadObj["url"].toString();
    typeStr = downloadObj["type"].toString();

    Download jsonDownload = {
      .mType = typeStr,
      .mURL = urlStr,
      .mFileHashes = fileHashes,
    };

    resDownloads.push_back(jsonDownload);

  }

  std::list<Mod> dependencies;
  auto deps = obj["dependencies"].toArray();
  foreach( QJsonValue dep, deps )
  {
    QJsonObject depObj = dep.toObject();
    processModJson( dependencies, depObj );
  }

  Mod mod = {
    .mID = obj["_id"].toString(),
    .mDescription = obj["description"].toString(),
    .mGameVersion = obj["gameVersion"].toString(),
    .mName = obj["name"].toString(),
    .mVersion = obj["version"].toString(),
    .mDownloads = resDownloads,
    .mDependencies = dependencies,
  };

  res.emplace_back(mod);
}

bool BeatModsV1::downloadModFile( Download download, QFile& file )
{
  // Download a mod through the api and save it to the temporary dir
  // TODO: Hard coding approved here, maybe that should be on Settings
  QString urlStr = "https://beatmods.com/" + download.mURL;
  QTextStream qOut( stdout );

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
    qOut << "ERROR: Failed to download mod file from beatmods\n";
    return {};
  }

  return true;
}

