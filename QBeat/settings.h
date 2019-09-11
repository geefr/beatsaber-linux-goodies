#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class Settings : public QSettings
{
public:
  static Settings instance;

  /**
   * Directory containing shell scripts
   * Defaults to application directory
   */
  void scriptDir(QString dir);
  QString scriptDir() const;

  /**
   * Game version
   */
  void gameVersion(QString version);
  QString gameVersion() const;

  /**
   * Game Installation Type
   * steam or oculus
   */
  void gameType(QString type);
  QString gameType() const;

#ifndef Q_OS_WIN32
  /**
   * Wine prefix for running BSIPA
   */
  void winePrefix(QString prefix);
  QString winePrefix() const;

  /**
   * Base proton installation to use
   * with beat saber
   */
  void bsProtonDir(QString dir);
  QString bsProtonDir() const;
#endif

  /**
   * Beat Saber installation directory
   */
  void bsInstall(QString dir);
  QString bsInstall() const;

  // Keys for settings
  static const char* kScriptDir;
  static const char* kGameVersion;
  static const char* kGameType;
  static const char* kWinePrefix;
  static const char* kBSProtonDir;
  static const char* kBSInstall;
private:
  Settings();
  ~Settings();
};

#endif
