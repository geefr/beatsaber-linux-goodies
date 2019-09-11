#include "actions.h"
#include <QProcess>
#include <QFile>
#include <QTemporaryDir>
#include <QCoreApplication>

#include "settings.h"
#include "apis/beatmodsv1.h"
#include "util.h"
#include <iostream>

Actions::Actions(QObject *parent) : QObject(parent)
{

}

void Actions::printAllConfig(QTextStream& qOut) {
  auto keys = Settings::instance.allKeys();
  for( auto& key : keys ) printConfig(qOut, key);
}

void Actions::printConfig(QTextStream& qOut, QString key) {
  qOut << key << " : " << Settings::instance.value(key).toString() << "\n";
}

void Actions::setConfig(QString key, QString val) {
  if( Settings::instance.contains(key) ) {
    Settings::instance.setValue(key, val);
  }
}

bool Actions::isWinePrefixValid()
{
  QProcess process;
  QTextStream qOut( stdout );

  auto script = QCoreApplication::applicationDirPath() + "/bs-linux-is-wine-valid.sh";
  if( !QFile::exists(script) )
  {
    qOut << "ERROR: Failed to find script: " << script << "\n";
    return false;
  }

  qOut << "Executing: " << script << "\n";

  process.setWorkingDirectory(QCoreApplication::applicationDirPath());
  process.start(script, {Settings::instance.winePrefix()});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  return process.exitCode() == EXIT_SUCCESS;
}


bool Actions::setupWine()
{
  QProcess process;
  QTextStream qOut( stdout );

  auto script = QCoreApplication::applicationDirPath()  + "/bs-linux-setup-wine.sh";
  if( !QFile::exists(script) )
  {
    qOut << "ERROR: Failed to find script: " << script << "\n";
    return false;
  }

  qOut << "Executing: " << script << "\n";

  process.setWorkingDirectory(QCoreApplication::applicationDirPath() );
  process.start(script, {Settings::instance.winePrefix()});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  return process.exitCode() == EXIT_SUCCESS;
}

#ifndef Q_OS_WIN32
bool Actions::patchBeatSaber()
{
  QProcess process;
  QTextStream qOut( stdout );

  auto script = QCoreApplication::applicationDirPath()  + "/bs-linux-modfix.sh";
  if( !QFile::exists(script) )
  {
    qOut << "ERROR: Failed to find script: " << script << "\n";
    return false;
  }

  qOut << "Executing: " << script << "\n";

  process.setWorkingDirectory(QCoreApplication::applicationDirPath() );
  process.start(script, {
    Settings::instance.bsInstall(),
    Settings::instance.bsProtonDir(),
    Settings::instance.winePrefix()
  });
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  return process.exitCode() == EXIT_SUCCESS;
}
#endif

std::list<Mod> Actions::listAvailableMods()
{
  // TODO: Support for other apis
  // TODO: Support for github downloads/similar, for things that aren't approved on beatmods yet (Obviously at the users override)
  BeatModsV1 api;
  auto mods = api.getMods({{
    "gameVersion", Settings::instance.gameVersion()},
  });

  return mods;
}

Mod Actions::getNamedMod( QString name )
{
  BeatModsV1 api;
  auto mods = api.getMods({
    {"gameVersion", Settings::instance.gameVersion()},
    {"name", name},
  });

  if( mods.empty() ) return {};
  return *(mods.begin());
}

bool Actions::installMod( Mod mod, bool includeDependencies )
{
  // Alright this is the first tough one

  // Download the mod's payload to a temporary file
  QTemporaryDir tempDir;
  QTextStream qOut( stdout );
  if (!tempDir.isValid()) {
      qOut << "ERROR: Failed to create temporary dir for mod download\n";
      return false;
  }

  if( !downloadMod(mod, tempDir.path(), includeDependencies) ) {
    return false;
  }

  // Iterate over the downloaded files
  QDir dir(tempDir.path());
  dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  auto files = dir.entryList();

  for( auto& file : files ) {
    auto fullPath = dir.path() + "/" + file;
    if( !Util::extractArchive(fullPath, Settings::instance.bsInstall()) ) {
      qOut << "ERROR: Failed to extract archive: " << fullPath << "\n";
      return false;
    }
  }

  return true;
}

bool Actions::downloadMod( Mod mod, QString directory, bool includeDependencies )
{
  // Alright this is the first tough one

  // Download the mod's payload to a temporary file
  // TODO: I'm assuming here that all the files are .zip archives just to get the BSIPA download working

  // TODO: Make this reliable and such
  BeatModsV1 api;
  QTextStream qOut( stdout );
  for( auto download : mod.mDownloads )
  {
    if( download.mType != Settings::instance.gameType() &&
        download.mType != "universal" ) continue;

    auto split = download.mURL.split("/", QString::SplitBehavior::SkipEmptyParts);

    QFile tempFile(directory + "/" + split.last());
    tempFile.open(QFile::OpenModeFlag::ReadWrite);
    if( !tempFile.isOpen() ) {
      qOut << "ERROR: Failed to open file for mod download: " + tempFile.fileName() << "\n";
      return false;
    }

    if( !api.downloadModFile( download, tempFile ) ) {
      qOut << "ERROR: Failed to download mod: " + mod.mName << "\n";
      return false;
    }

    tempFile.close();
  }

  if( includeDependencies ) {
    for( auto& dep : mod.mDependencies ) {
      // If multiple mods are installed, requiring the same dependency they may require different
      // versions.
      // So here we need to explicitly fetch the latest version of the dependency from the api,
      // and if we can't find it then fall back to the information provided as part of the mod we're installing.
      auto latestDep = api.getMods({{"name", dep.mName}, {"gameVersion", Settings::instance.gameVersion()}});
      if( latestDep.empty() ) {
        qOut << "ERROR: Failed to fetch " << dep.mName << " for mod dependency\n";
        return false;
        /*
        // Fall over to the listed version
        if( !downloadMod(dep, directory, includeDependencies ) ) {
          return false;
        }*/
      } else {
        // Download the latest available version of the mod
        if( !downloadMod(latestDep.front(), directory, includeDependencies) ) {
          return false;
        }
      }
    }
  }

  // TODO: Beat saber dir from settings/cache
  // TODO: Don't assume everything is a zip
  // TODO: Actually validate the mod after the download - skipping the install if something doesn't match would be kind useful here
  return true;
}

std::list<Mod> Actions::filterModsToVersion(std::list<Mod> mods, QString version)
{
  std::list<Mod> res;
  std::copy_if(mods.begin(), mods.end(), std::back_inserter(res), [&](const Mod& mod) {
    return mod.mGameVersion == version;
  });
  return res;
}







