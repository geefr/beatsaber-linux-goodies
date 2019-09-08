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

    //qDebug() << obj;

    auto downloads = obj["downloads"].toArray();
    std::list<Download> resDownloads;

    foreach( QJsonValue download, downloads)
    {
      std::map<QString,QString> fileHashes;
      QString typeStr;
      QString urlStr;

      auto downloadObj = download.toObject();

      // TODO: This bit about downloads is a total mess, sort it out
      urlStr = download["url"].toString();
      typeStr = download["type"].toString();
      //qDebug() << "Download URL: " << downloadUrl.toString();

      /*
      foreach( QJsonValue entry, downloadObj )
      {
        qDebug() << "File: " << entry["file"];
        qDebug() << "Hash: " << entry["hash"];
      }

      "downloads":[
        {"hashMd5":[
          {"file":"IPA/Data/Managed/I18N.dll","hash":"aa156a789840ef85e6c0fd08b27a18a6"},
          {"file":"IPA/Data/Managed/I18N.West.dll","hash":"a75f9881a0e9b61ad1d02bdba0406468"},
          {"file":"IPA/Data/Managed/IPA.Injector.dll","hash":"b8386fcab234e36ae3953ad2f613db14"},
          {"file":"IPA/Data/Managed/IPA.Injector.pdb","hash":"5f226cbc82c522890e26f9707667f5c8"},
          {"file":"IPA/Data/Managed/IPA.Loader.dll","hash":"b4e82eacb40b986def005670c69b8c57"},
          {"file":"IPA/Data/Managed/IPA.Loader.pdb","hash":"b64673feeec5b66389fc8a9c2b4eeb3a"},
          {"file":"IPA/Data/Managed/IPA.Loader.xml","hash":"fdce9150f8e8ae7c3c33458943fb6fe9"},
          {"file":"IPA/Data/Managed/Microsoft.CSharp.dll","hash":"a8f13d4df59ebbcae3d07daaeab739af"},
          {"file":"IPA/Data/Managed/System.Runtime.Serialization.dll","hash":"c739259c418dad08468a4def4c4d1486"},
          {"file":"IPA/Libs/0Harmony.1.2.0.1.dll","hash":"e11a2fa00d46a40c485b41126cd7d1c8"},
          {"file":"IPA/Libs/Ionic.Zip.1.9.1.8.dll","hash":"6ded8fcbf5f1d9e422b327ca51625e24"},
          {"file":"IPA/Libs/Mono.Cecil.0.10.4.0.dll","hash":"bda5cf9930c3bb1976c7484c7f280cba"},
          {"file":"IPA/Libs/Mono.Cecil.Mdb.0.10.4.0.dll","hash":"df8837d3efdaa1405200f529f06f36b1"},
          {"file":"IPA/Libs/Mono.Cecil.Pdb.0.10.4.0.dll","hash":"aeff6fb43e6604178e905465463eb546"},
          {"file":"IPA/Libs/Mono.Cecil.Rocks.0.10.4.0.dll","hash":"b1de908243feac14049ddeefb858ef33"},
          {"file":"IPA/Libs/Newtonsoft.Json.12.0.0.0.dll","hash":"4df6c8781e70c3a4912b5be796e6d337"},
          {"file":"IPA/Libs/SemVer.1.2.0.0.dll","hash":"f0a2d65976e56446c84578e8b8030a34"},
          {"file":"IPA/winhttp.dll","hash":"f25171a12e8b375164ea79c09730e931"},
          {"file":"IPA.exe","hash":"033c5e715a0fe5f44fed9b2215aeeb46"},
          {"file":"IPA.exe.config","hash":"113aab26517f62b45112e50b3889e63e"}
        ],
        "type":"universal",
        "url":"/uploads/5d682be2be34f5432f209d8e/universal/BSIPA-3.13.0.zip"}
      ]*/

      //qDebug() << download.toString();
/*
      auto fileHashArr = fileHashObj.toArray();
      foreach( QJsonValue fileHash, fileHashArr ) {
        fileHashes[fileHash["file"].toString()] = fileHash["hash"].toString();
      }
*/
      Download jsonDownload = {
        .mType = typeStr,
        .mURL = urlStr,
        .mFileHashes = fileHashes,
      };

      resDownloads.push_back(jsonDownload);

    }

    Mod mod = {
      .mID = obj["_id"].toString(),
      .mDescription = obj["description"].toString(),
      .mGameVersion = obj["gameVersion"].toString(),
      .mName = obj["name"].toString(),
      .mDownloads = resDownloads,
    };

    res.emplace_back(mod);
  }

  return res;
}

bool BeatModsV1::downloadModFile( Download download, QFile& file )
{
  // Download a mod through the api and save it to the temporary dir
  // TODO: Hard coding approved here, maybe that should be on Settings
  QString urlStr = "https://beatmods.com/" + download.mURL;

  qDebug() << "Download url: " << urlStr;


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
    qDebug() << "ERROR: Failed to download mod file from beatmods";
    return {};
  }

  return true;
}

