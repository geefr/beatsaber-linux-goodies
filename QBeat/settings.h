#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings
{
  /**
   * Directory containing shell scripts
   * Defaults to application directory
   * Can be overriden by QBEAT_SCRIPT_DIR for testing
   */
  static QString scriptDir;

  /**
   * Initialise settings
   * Check for environment overrides and such
   * Called from main()
   */
  static void initSettings();
};

#endif
