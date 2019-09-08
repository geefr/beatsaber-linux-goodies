#include "actions.h"
#include <QProcess>
#include <QDebug>
#include <QFile>

#include "settings.h"
#include "apis/beatmodsv1.h"

Actions::Actions(QObject *parent) : QObject(parent)
{

}

bool Actions::isWinePrefixValid( QString winePrefix )
{
  QProcess process;


  auto script = Settings::scriptDir + "/bs-linux-is-wine-valid.sh";
  if( !QFile::exists(script) )
  {
    qDebug() << "ERROR: Failed to find script: " << script;
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
    qDebug() << "ERROR: Failed to find script: " << script;
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
    qDebug() << "ERROR: Failed to find script: " << script;
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

void Actions::listAvailableMods()
{
  // TODO: Support for other apis
  // TODO: Support for github downloads/similar, for things that aren't approved on beatmods yet (Obviously at the users override)
  BeatModsV1 api;
  auto mods = api.getAllMods();

  mods = filterModsToVersion(mods, Settings::gameVersion);

  for( auto mod : mods )
  {
    qDebug() << mod.mName;
    qDebug() << mod.mID;
    qDebug() << mod.mGameVersion;
    qDebug() << mod.mDescription;
    qDebug() << "\n";
  }

}

std::list<Mod> Actions::filterModsToVersion(std::list<Mod> mods, QString version)
{
  std::list<Mod> res;
  std::copy_if(mods.begin(), mods.end(), std::back_inserter(res), [&](const Mod& mod) {
    return mod.mGameVersion == version;
  });
  return res;
}







