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
  connect(&mNetMan, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
          this, SLOT(onSslErrors(QNetworkReply*, const QList<QSslError>&)));
}

void BeatModsV1::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
  QTextStream qOut( stdout );

  qOut << "ERROR: SSL Errors during request: \n";

  for( auto& error : errors ) {
    qOut << "     : " << error.errorString() << "\n";
  }
}

std::list<Mod> BeatModsV1::getMods( std::map<QString, QString> filters)
{
  QTextStream qOut( stdout );
  // TODO: Hard coding approved here, maybe that should be on Settings
  QString urlStr = "https://beatmods.com/api/v1/mod?status=approved";
  for( auto& filter : filters )
  {
    urlStr = urlStr + "&" + filter.first + "=" + QUrl::toPercentEncoding(filter.second);
  }

  QUrl url(urlStr);
  QNetworkRequest request(url);
  sslWorkarounds(request);

  std::unique_ptr<QNetworkReply> response (mNetMan.get(request));

  QEventLoop loop;
  QObject::connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();
  if( !checkResponse(response) ) return {};

  auto responseData = response->readAll();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
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

    auto hashes = downloadObj["hashMd5"].toArray();
    foreach( QJsonValue hash, hashes) {
      auto hashObj = hash.toObject();
      auto file = hashObj["file"].toString();
      auto md5 = hashObj["hash"].toString();
      fileHashes[file] = md5;
    }

/*

    "downloads":[
      {"type":"universal","url":"/uploads/5d815bfe626a455aff49554e/universal/LibConf-1.0.0.zip",
        "hashMd5":[
          {"hash":"31f95ec35fe1573ddda70db877298d0a","file":"Libs/LibConf.1.0.0.0.dll"},
          {"hash":"986d90bab82d2389fd77849b0308a032","file":"Libs/LibConf.xml"},
          {"hash":"a6cba0995cc4e8430dd2677fe058e5f1","file":"Plugins/LibConf.manifest"}
      ]}
    ],
    "category":"Libraries","required":false,"dependencies":["5d681028be34f5432f209d42","5d6bfbedbe34f5432f209e5e","5d6bfc1abe34f5432f209e61"]}],"_id":"5d8835c3626a455aff495585"}

*/
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
  QString urlStr = "https://beatmods.com";
  if( download.mURL[0] != '/' ) urlStr += "/";
  urlStr += download.mURL;

  QTextStream qOut( stdout );

  QUrl url(urlStr);
  QNetworkRequest request(url);
  sslWorkarounds(request);
  std::unique_ptr<QNetworkReply> response (mNetMan.get(request));

  connect(response.get(), &QNetworkReply::readyRead, this, [&]() {
    file.write(response->readAll());
  });

  QEventLoop loop;
  connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();

  if( !checkResponse(response) ) return false;
  return true;
}

void BeatModsV1::sslWorkarounds( QNetworkRequest& request ) {
  QTextStream qOut( stdout );
  auto sslConf = QSslConfiguration::defaultConfiguration();

  if( qgetenv("QBEAT_DISABLE_SSLVERIFY").isEmpty() == false ) {
    qOut << "WARNING: SSL peer verification has been disabled\n";
    sslConf.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
  }
  if( qgetenv("QBEAT_ALLOW_ANY_SSL").isEmpty() == false ) {
    qOut << "WARNING: Any SSL protocol has been allowed\n";
    sslConf.setProtocol(QSsl::AnyProtocol);
  }

  request.setSslConfiguration(sslConf);
}

bool BeatModsV1::checkResponse( std::unique_ptr<QNetworkReply>& response ) {
  QTextStream qOut( stdout );
  if( response->error() != QNetworkReply::NoError )
  {
    qOut << "ERROR: Failed to fetch mod list from beatmods: " << static_cast<int>(response->error()) << ": URL: " << response->url().toString() << "\n";
    return false;
  }
}
