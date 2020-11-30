using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Text.Json;
using Beataroni.Models.BeatMods;

namespace Beataroni.Services
{
  public class BeatModsV1
  {
    public static readonly Dictionary<string, string> DefaultFilters = new Dictionary<string, string>()
    {
      {"status","approved"},
    };

    /// API endpoints/urls
    /// Note: You MUST not have '//' in the resulting URLs, beatmods can't handle this
    private static readonly string APIRoot = "https://beatmods.com/api/v1";
    private static readonly string APIVersion = "version";
    private static readonly string APIMod = "mod";

    /// Fetch list of beatsaber/mod versions
    /// List will be returned in order sent from server - Should be newest -> oldest
    public static IList<string> FetchBSVersions()
    {
      var endpoint = $"{APIRoot}/{APIVersion}";

      // TODO: The usual junk here, maybe a common method for doing a basic GET+deserialise as we're doing lots of those
      try
      {
        var req = WebRequest.Create(endpoint);
        req.Method = "GET";

        var resp = req.GetResponse();

        var json = "";
        using (Stream s = resp.GetResponseStream())
        {
          var reader = new StreamReader(s, Encoding.UTF8);
          json = reader.ReadToEnd();
        }

        List<string> result = JsonSerializer.Deserialize<List<string>>(json);
        return result;
      } catch (WebException e)
      {
        Console.WriteLine($"Error fetching BS versions: {e.Message}");
        return null;
      }
    }

    /// Fetch list of mods for a set of filters
    /// Typical filters would be game version, installation type, mod status
    public static IList<Mod> FetchMods( Dictionary<string, string> filters )
    {
      var endpoint = $"{APIRoot}/{APIMod}";

      if( filters != null && filters.Count > 0 )
      {
        endpoint += "?";
        var first = true;
        foreach( var f in filters )
        {
          if( !first )
          {
            endpoint += "&";
          }
          endpoint += $"{f.Key}={f.Value}";
          first = false;
        }
      }

      // TODO: The usual junk here, maybe a common method for doing a basic GET+deserialise as we're doing lots of those
      try
      {
        var req = WebRequest.Create(endpoint);
        req.Method = "GET";

        var resp = req.GetResponse();

        var json = "";
        using (Stream s = resp.GetResponseStream())
        {
          var reader = new StreamReader(s, Encoding.UTF8);
          json = reader.ReadToEnd();
        }

        List<Mod> result = JsonSerializer.Deserialize<List<Mod>>(json);
        return result;
      } catch (WebException e)
      {
        Console.WriteLine($"Error fetching BS Mods: {e.Message}");
        return null;
      }
    }

    /// Download a mod to a temporary file
    /// Returns either a path to the file, or null if download failed for some reason
    public static string DownloadMod( Mod modToInstall, string BSDir )
    {
      throw new NotImplementedException();
    }
  }
}
    

// BeatModsV1::BeatModsV1()
// {
//   connect(&mNetMan, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
//           this, SLOT(onSslErrors(QNetworkReply*, const QList<QSslError>&)));
// }

// void BeatModsV1::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
//   QTextStream qOut( stdout );

//   qOut << "ERROR: SSL Errors during request: \n";

//   for( auto& error : errors ) {
//     qOut << "     : " << error.errorString() << "\n";
//   }
// }

// std::list<Mod> BeatModsV1::getMods( std::map<QString, QString> filters)
// {
//   QTextStream qOut( stdout );
//   // TODO: Hard coding approved here, maybe that should be on Settings
//   QString urlStr = "https://beatmods.com/api/v1/mod?status=approved";
//   for( auto& filter : filters )
//   {
//     urlStr = urlStr + "&" + filter.first + "=" + QUrl::toPercentEncoding(filter.second);
//   }

//   QUrl url(urlStr);
//   QNetworkRequest request(url);
//   sslWorkarounds(request);

//   std::unique_ptr<QNetworkReply> response (mNetMan.get(request));

//   QEventLoop loop;
//   QObject::connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
//   loop.exec();
//   if( !checkResponse(response) ) return {};

//   auto responseData = response->readAll();
//   QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
//   QJsonObject jsonObj = jsonDoc.object();
//   QJsonArray jsonArr = jsonDoc.array();

//   std::list<Mod> res;


//   foreach( QJsonValue val, jsonArr )
//   {
//     QJsonObject obj = val.toObject();
//     processModJson( res, obj );
//   }

//   return res;
// }

// void BeatModsV1::processModJson( std::list<Mod>& res, QJsonObject obj )
// {
//   auto downloads = obj["downloads"].toArray();
//   std::list<Download> resDownloads;

//   foreach( QJsonValue download, downloads)
//   {
//     std::map<QString,QString> fileHashes;
//     QString typeStr;
//     QString urlStr;

//     auto downloadObj = download.toObject();

//     // TODO: This bit about downloads is a total mess, sort it out
//     urlStr = downloadObj["url"].toString();
//     typeStr = downloadObj["type"].toString();

//     auto hashes = downloadObj["hashMd5"].toArray();
//     foreach( QJsonValue hash, hashes) {
//       auto hashObj = hash.toObject();
//       auto file = hashObj["file"].toString();
//       auto md5 = hashObj["hash"].toString();
//       fileHashes[file] = md5;
//     }

// /*

//     "downloads":[
//       {"type":"universal","url":"/uploads/5d815bfe626a455aff49554e/universal/LibConf-1.0.0.zip",
//         "hashMd5":[
//           {"hash":"31f95ec35fe1573ddda70db877298d0a","file":"Libs/LibConf.1.0.0.0.dll"},
//           {"hash":"986d90bab82d2389fd77849b0308a032","file":"Libs/LibConf.xml"},
//           {"hash":"a6cba0995cc4e8430dd2677fe058e5f1","file":"Plugins/LibConf.manifest"}
//       ]}
//     ],
//     "category":"Libraries","required":false,"dependencies":["5d681028be34f5432f209d42","5d6bfbedbe34f5432f209e5e","5d6bfc1abe34f5432f209e61"]}],"_id":"5d8835c3626a455aff495585"}

// */
//     Download jsonDownload = {
//       .mType = typeStr,
//       .mURL = urlStr,
//       .mFileHashes = fileHashes,
//     };

//     resDownloads.push_back(jsonDownload);

//   }

//   std::list<Mod> dependencies;
//   auto deps = obj["dependencies"].toArray();
//   foreach( QJsonValue dep, deps )
//   {
//     QJsonObject depObj = dep.toObject();
//     processModJson( dependencies, depObj );
//   }

//   Mod mod = {
//     .mID = obj["_id"].toString(),
//     .mDescription = obj["description"].toString(),
//     .mGameVersion = obj["gameVersion"].toString(),
//     .mName = obj["name"].toString(),
//     .mVersion = obj["version"].toString(),
//     .mDownloads = resDownloads,
//     .mDependencies = dependencies,
//   };

//   res.emplace_back(mod);
// }

// bool BeatModsV1::downloadModFile( Download download, QFile& file )
// {
//   // Download a mod through the api and save it to the temporary dir
//   // TODO: Hard coding approved here, maybe that should be on Settings
//   QString urlStr = "https://beatmods.com";
//   if( download.mURL[0] != '/' ) urlStr += "/";
//   urlStr += download.mURL;

//   QTextStream qOut( stdout );

//   QUrl url(urlStr);
//   QNetworkRequest request(url);
//   sslWorkarounds(request);
//   std::unique_ptr<QNetworkReply> response (mNetMan.get(request));

//   connect(response.get(), &QNetworkReply::readyRead, this, [&]() {
//     file.write(response->readAll());
//   });

//   QEventLoop loop;
//   connect(response.get(), SIGNAL(finished()), &loop, SLOT(quit()));
//   loop.exec();

//   if( !checkResponse(response) ) return false;
//   return true;
// }

// void BeatModsV1::sslWorkarounds( QNetworkRequest& request ) {
//   QTextStream qOut( stdout );
//   auto sslConf = QSslConfiguration::defaultConfiguration();

//   if( qgetenv("QBEAT_DISABLE_SSLVERIFY").isEmpty() == false ) {
//     qOut << "WARNING: SSL peer verification has been disabled\n";
//     sslConf.setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
//   }
//   if( qgetenv("QBEAT_ALLOW_ANY_SSL").isEmpty() == false ) {
//     qOut << "WARNING: Any SSL protocol has been allowed\n";
//     sslConf.setProtocol(QSsl::AnyProtocol);
//   }

//   request.setSslConfiguration(sslConf);
// }

// bool BeatModsV1::checkResponse( std::unique_ptr<QNetworkReply>& response ) {
//   QTextStream qOut( stdout );
//   if( response->error() != QNetworkReply::NoError )
//   {
//     qOut << "ERROR: Failed to fetch mod list from beatmods: " << static_cast<int>(response->error()) << ": URL: " << response->url().toString() << "\n";
//     return false;
//   }
//   return true;
// }
