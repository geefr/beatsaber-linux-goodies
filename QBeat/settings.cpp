#include "settings.h"

QString Settings::scriptDir = ".";

void Settings::initSettings()
{
  auto scriptOverride = qgetenv("QBEAT_SCRIPT_DIR");
  if( scriptOverride.size() ) {
    Settings::scriptDir = QString::fromUtf8(scriptOverride);
  }
}
