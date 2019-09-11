#include "settings.h"

Settings Settings::instance;
const char* Settings::kScriptDir = "scriptDir";
const char* Settings::kGameVersion = "gameVersion";
const char* Settings::kGameType = "gameType";
const char* Settings::kWinePrefix = "winePrefix";
const char* Settings::kBSProtonDir = "bsProtonDir";
const char* Settings::kBSInstall = "bsInstall";

Settings::Settings()
  : QSettings ("gfrancisdev", "QBeat")
{
  // Init settings if needed
  if( !contains(kScriptDir) ) setValue(kScriptDir, ".");
  if( !contains(kGameVersion) ) setValue(kGameVersion, "1.3.0");
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

void Settings::scriptDir(QString dir) {
  setValue(kScriptDir, dir);
}

QString Settings::scriptDir() const {
  return value(kScriptDir).toString();
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
  setValue(kWinePrefix, prefix);
}

QString Settings::winePrefix() const {
  return value(kWinePrefix).toString();
}

void Settings::bsProtonDir(QString dir) {
  setValue(kBSProtonDir, dir);
}

QString Settings::bsProtonDir() const {
  return value(kBSProtonDir).toString();
}
#endif

void Settings::bsInstall(QString dir) {
  setValue(kBSInstall, dir);
}

QString Settings::bsInstall() const {
  return value(kBSInstall).toString();
}
