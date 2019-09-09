#include "actions.h"
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QTemporaryDir>

#include "settings.h"
#include "apis/beatmodsv1.h"
#include "util.h"
#include <iostream>

Actions::Actions(QObject *parent) : QObject(parent)
{

}

bool Actions::isWinePrefixValid( QString winePrefix )
{
  QProcess process;


  auto script = Settings::scriptDir + "/bs-linux-is-wine-valid.sh";
  if( !QFile::exists(script) )
  {
    qDebug() << "ERROR: Failed to find script, is QBEAT_SCRIPT_DIR set?: " << script;
    return false;
  }

  process.setWorkingDirectory(Settings::scriptDir);
  process.start(script, {winePrefix});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  //QByteArray str = process.readAll();
  //qDebug() << str;

  return process.exitCode() == EXIT_SUCCESS;
}


bool Actions::setupWine( QString winePrefix )
{
  QProcess process;

  auto script = Settings::scriptDir + "/bs-linux-setup-wine.sh";
  if( !QFile::exists(script) )
  {
    qDebug() << "ERROR: Failed to find script, is QBEAT_SCRIPT_DIR set?: " << script;
    return false;
  }

  process.setWorkingDirectory(Settings::scriptDir);
  process.start(script, {winePrefix});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  //QByteArray str = process.readAll();
  //qDebug() << str;

  return process.exitCode() == EXIT_SUCCESS;
}


bool Actions::linuxModFix( QString bsInstall, QString protonInstall, QString winePrefix )
{
  QProcess process;

  auto script = Settings::scriptDir + "/bs-linux-modfix.sh";
  if( !QFile::exists(script) )
  {
    qDebug() << "ERROR: Failed to find script, is QBEAT_SCRIPT_DIR set?: " << script;
    return false;
  }

  process.setWorkingDirectory(Settings::scriptDir);
  process.start(script, {bsInstall, protonInstall, winePrefix});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

  //QByteArray str = process.readAll();
  //qDebug() << str;

  return process.exitCode() == EXIT_SUCCESS;
}

std::list<Mod> Actions::listAvailableMods()
{
  // TODO: Support for other apis
  // TODO: Support for github downloads/similar, for things that aren't approved on beatmods yet (Obviously at the users override)
  BeatModsV1 api;
  auto mods = api.getMods({{
    "gameVersion", Settings::gameVersion},
  });

  return mods;
}

Mod Actions::getNamedMod( QString name )
{
  BeatModsV1 api;
  auto mods = api.getMods({
    {"gameVersion", Settings::gameVersion},
    {"name", name},
  });

  if( mods.empty() ) return {};
  return *(mods.begin());
}

bool Actions::installMod( Mod mod, QString directory )
{
  // Alright this is the first tough one
  // TODO: Fetch a list of dependencies
  // TODO: Install each of the dependencies

  // Download the mod's payload to a temporary file
  QTemporaryDir tempDir;
  if (!tempDir.isValid()) {
      qDebug() << "ERROR: Failed to create temporary dir for mod download";
      return false;
  }

  if( !downloadMod(mod,tempDir.path()) ) {
    return false;
  }

  // Iterate over the downloaded files
  QDir dir(tempDir.path());
  dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  auto files = dir.entryList();

  for( auto& file : files ) {
    auto fullPath = dir.path() + "/" + file;
    if( !Util::extractArchive(fullPath, directory) ) {
      qDebug() << "ERROR: Failed to extract archive: " << fullPath;
      return false;
    }
  }

  return true;
}

bool Actions::downloadMod( Mod mod, QString directory )
{
  // Alright this is the first tough one
  // TODO: Fetch a list of dependencies
  // TODO: Download each of the dependencies

  // Download the mod's payload to a temporary file
  // TODO: I'm assuming here that all the files are .zip archives just to get the BSIPA download working

  // TODO: Make this reliable and such
  for( auto download : mod.mDownloads )
  {
    auto split = download.mURL.split("/", QString::SplitBehavior::SkipEmptyParts);

    QFile tempFile(directory + "/" + split.last());
    tempFile.open(QFile::OpenModeFlag::ReadWrite);
    if( !tempFile.isOpen() ) {
      qDebug() << "ERROR: Failed to open file for mod download: " + tempFile.fileName();
      return false;
    }

    BeatModsV1 api;
    if( !api.downloadModFile( download, tempFile ) ) {
      qDebug() << "ERROR: Failed to download mod: " + mod.mName;
      return false;
    }

    tempFile.close();
  }

  // TODO: Decompress the mod download into the beatsaber directory
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







