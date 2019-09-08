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

std::list<Mod> BeatModsV1::getAllMods()
{
  QUrl url("https://beatmods.com/api/v1/mod?status=approved");
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

    Mod mod = {
      .mID = obj["_id"].toString(),
      .mDescription = obj["description"].toString(),
      .mGameVersion = obj["gameVersion"].toString(),
      .mName = obj["name"].toString(),
    };

    res.emplace_back(mod);
  }

  return res;
}

