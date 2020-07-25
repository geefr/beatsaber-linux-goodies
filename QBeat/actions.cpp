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

bool Actions::isWinePrefixValid()
{
  QProcess process;
  QTextStream qOut( stdout );

  QFile script( QCoreApplication::applicationDirPath() + "/bs-linux-is-wine-valid.sh" );
  if( !script.exists() )
  {
    qOut << "ERROR: Failed to find script: " << script.fileName() << "\n";
    return false;
  }
  if( !script.permissions().testFlag(QFile::Permission::ExeOwner) ) {
    qOut << "ERROR: Scipt is not executable: " << script.fileName() << "\n";
    return false;
  }

  qOut << "Executing: " << script.fileName() << "\n";

  process.setWorkingDirectory(QCoreApplication::applicationDirPath());
  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
  process.start(script.fileName(), {Settings::instance.winePrefix()});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  if( process.exitCode() != EXIT_SUCCESS ) {
      qOut << "Script execution failed: \n" << process.readAll() << "\n";
  }

  return process.exitCode() == EXIT_SUCCESS;
}


bool Actions::setupWine()
{
  QProcess process;
  QTextStream qOut( stdout );

  QFile script( QCoreApplication::applicationDirPath()  + "/bs-linux-setup-wine.sh" );
  if( !script.exists() )
  {
    qOut << "ERROR: Failed to find script: " << script.fileName() << "\n";
    return false;
  }
  if( !script.permissions().testFlag(QFile::Permission::ExeOwner) ) {
    qOut << "ERROR: Scipt is not executable: " << script.fileName() << "\n";
    return false;
  }

  qOut << "Executing: " << script.fileName() << "\n";

  process.setWorkingDirectory(QCoreApplication::applicationDirPath() );
  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
  process.start(script.fileName(), {Settings::instance.winePrefix()});
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  if( process.exitCode() != EXIT_SUCCESS ) {
      qOut << "Script execution failed: \n" << process.readAll() << "\n";
  }

  return process.exitCode() == EXIT_SUCCESS;
}

#ifndef Q_OS_WIN32
bool Actions::patchBeatSaber()
{
  QProcess process;
  QTextStream qOut( stdout );

  QFile script( QCoreApplication::applicationDirPath()  + "/bs-linux-modfix.sh" );
  if( !script.exists() )
  {
    qOut << "ERROR: Failed to find script: " << script.fileName() << "\n";
    return false;
  }
  if( !script.permissions().testFlag(QFile::Permission::ExeOwner) ) {
    qOut << "ERROR: Scipt is not executable: " << script.fileName() << "\n";
    return false;
  }

  qOut << "Executing: " << script.fileName() << "\n";
  process.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
  process.setWorkingDirectory(QCoreApplication::applicationDirPath() );
  process.start(script.fileName(), {
    Settings::instance.bsInstall(),
    Settings::instance.winePrefix()
  });
  process.waitForStarted(-1);
  process.waitForFinished(-1);

  if( process.exitCode() != EXIT_SUCCESS ) {
      qOut << "Script execution failed: \n" << process.readAll() << "\n";
  }

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

bool Actions::installAllMods()
{
  // Alright this is the first tough one

  // Download the mod's payload to a temporary file
  BeatModsV1 api;
  QTemporaryDir tempDir;
  QTextStream qOut( stdout );
  if (!tempDir.isValid()) {
      qOut << "ERROR: Failed to create temporary dir for mod download\n";
      return false;
  }

  auto mods = api.getMods({{
    "gameVersion", Settings::instance.gameVersion()},
  });

  for( auto& mod : mods ) {
    // Don't include dependencies, we're downloading everything so it should be fine right?
    if( !downloadMod(mod, tempDir.path(), false) ) {
      qOut << "ERROR: Failed to download mod: " << mod.mName << "\n";
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

    qOut << "Installed mod: " << mod.mName << "\n";
    qOut.flush();
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

  // TODO: Don't assume everything is a zip
  // TODO: Actually validate the mod after the download - skipping the install if something doesn't match would be kind useful here
  return true;
}

bool Actions::validateMod(Mod mod, bool includeDependencies )
{
  BeatModsV1 api;
  QTextStream qOut( stdout );
  for( auto download : mod.mDownloads )
  {
    if( download.mType != Settings::instance.gameType() &&
        download.mType != "universal" ) continue;

    if( download.mFileHashes.empty() ) {
      qOut << "ERROR: Mod doesn't list any file hashes\n";
      return false;
    }

    auto allMissing = true;
    auto anyMissing = false;

    for( auto& fileToHash : download.mFileHashes ) {
      QString path = fileToHash.first;
      Util::fixPath(path);

      QFile tempFile(Settings::instance.bsInstall() + "/" + path );
      tempFile.open(QFile::OpenModeFlag::ReadOnly);

      if( !tempFile.isOpen() ) {
        anyMissing = true;
        continue;
      }

      allMissing = false;

      QCryptographicHash hash(QCryptographicHash::Md5);
      if( !hash.addData(&tempFile) ) {
        qOut << "ERROR: Failed to calculate hash for file: " + tempFile.fileName() << "\n";
        return false;
      }

      auto md5Str = hash.result().toHex();

      qOut << "File: " << fileToHash.first << " Expected: " << fileToHash.second << " Got: " << md5Str << "\n";

      if( md5Str != fileToHash.second ) {
        qOut << "ERROR: File failed validation (" << fileToHash.first << "), please run QBeat --install \"" + mod.mName << "\"\n";
        return false;
      }
    }

    // If everything is missing then that's fine, the mod isn't installed
    // If only some are missing then the mod is invalid
    if( allMissing == false &&
        anyMissing == true ) {
      qOut << "ERROR: Failed to open a file for mod verification \n";
      return false;
    }
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
        if( !validateMod(latestDep.front(), includeDependencies) ) {
          return false;
        }
      }
    }
  }

  return true;
}

bool Actions::validateAllMods()
{
  BeatModsV1 api;
  QTemporaryDir tempDir;
  QTextStream qOut( stdout );
  if (!tempDir.isValid()) {
      qOut << "ERROR: Failed to create temporary dir for mod download\n";
      return false;
  }

  auto mods = api.getMods({{
    "gameVersion", Settings::instance.gameVersion()},
  });

  for( auto& mod : mods ) {
    // Don't include dependencies, we're downloading everything so it should be fine right?
    if( !validateMod(mod, false) ) {
      qOut << "ERROR: Mod validation failed, run QBeat --install " << mod.mName << " to fix\n";
      return false;
    }
  }

  return true;
}

bool Actions::removeMod(Mod mod)
{
  BeatModsV1 api;
  QTextStream qOut( stdout );
  for( auto download : mod.mDownloads )
  {
    if( download.mType != Settings::instance.gameType() &&
        download.mType != "universal" ) continue;

    if( download.mFileHashes.empty() ) {
      qOut << "ERROR: Mod doesn't list any file hashes\n";
      return false;
    }

    for( auto& fileToHash : download.mFileHashes ) {

      QString path = fileToHash.first;
      Util::fixPath(path);

      QFile tempFile(Settings::instance.bsInstall() + "/" + path );

      // Delete the file (and don't worry if it fails)
      tempFile.remove();
    }
  }

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







