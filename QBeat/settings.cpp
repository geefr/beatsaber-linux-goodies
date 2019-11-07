#include "settings.h"
#include <QDir>

Settings Settings::instance;
const char* Settings::kGameVersion = "gameVersion";
const char* Settings::kGameType = "gameType";
const char* Settings::kWinePrefix = "winePrefix";
const char* Settings::kBSProtonDir = "bsProtonDir";
const char* Settings::kBSInstall = "bsInstall";

Settings::Settings()
  : QSettings ("gfrancisdev", "QBeat")
{
  // Init settings if needed
  if( !contains(kGameVersion) ) setValue(kGameVersion, "1.5.0");
  if( !contains(kGameType) ) setValue(kGameType, "steam");
#ifndef Q_OS_WIN32
  if( !contains(kWinePrefix) ) setValue(kWinePrefix, "");
  if( !contains(kBSProtonDir) ) setValue(kBSProtonDir, "");
#endif
  if( !contains(kBSInstall) ) setValue(kBSInstall, "");
}

Settings::~Settings()
{
}

void Settings::setConfig(QString key, QString val) {
    if( key == kBSInstall ||
        key == kWinePrefix ||
        key == kBSProtonDir ) {
        sanitisePath(val);
    }
  if( contains(key) ) {
    setValue(key, val);
  }
}

void Settings::gameVersion(QString version) {
  setValue(kGameVersion, version);
}

QString Settings::gameVersion() const {
  return value(kGameVersion).toString();
}

void Settings::gameType(QString type) {
  setValue(kGameType, type);
}

QString Settings::gameType() const {
  return value(kGameType).toString();
}

#ifndef Q_OS_WIN32
void Settings::winePrefix(QString prefix) {
  sanitisePath(prefix);
  setValue(kWinePrefix, prefix);
}

QString Settings::winePrefix() const {
  return value(kWinePrefix).toString();
}

void Settings::bsProtonDir(QString dir) {
  sanitisePath(dir);
  setValue(kBSProtonDir, dir);
}

QString Settings::bsProtonDir() const {
  return value(kBSProtonDir).toString();
}
#endif

void Settings::bsInstall(QString dir) {
  sanitisePath(dir);
  setValue(kBSInstall, dir);
}

QString Settings::bsInstall() const {
  return value(kBSInstall).toString();
}

void Settings::sanitisePath( QString& path ) {
  if( path.isEmpty() ) return;
  if( path[0] == '~' ) {
    path.remove(0,1);
    path.prepend(QDir::homePath());
  }
}
