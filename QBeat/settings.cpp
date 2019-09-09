#include "settings.h"

QString Settings::scriptDir = ".";
QString Settings::gameVersion = "1.3.0";
QString Settings::gameType = "steam";

void Settings::initSettings()
{
  auto scriptOverride = qgetenv("QBEAT_SCRIPT_DIR");
  if( scriptOverride.size() ) {
    Settings::scriptDir = QString::fromUtf8(scriptOverride);
  }
}
