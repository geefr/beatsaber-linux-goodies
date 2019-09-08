#include "actions.h"
#include <QProcess>
#include <QDebug>
#include <QFile>

#include "settings.h"

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
