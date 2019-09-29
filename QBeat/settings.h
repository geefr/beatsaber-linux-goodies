#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class Settings : public QSettings
{
  Q_OBJECT
public:
  static Settings instance;

  /**
   * QML Property bindings
   * These setup the properties exposed in QML for the Settings class so that when fields
   * are changed the values will be updated in the application settings
   */
  Q_PROPERTY(QString gameVersion READ gameVersion WRITE gameVersion NOTIFY gameVersionChanged)
  Q_PROPERTY(QString gameType READ gameType WRITE gameType NOTIFY gameTypeChanged)
#ifndef Q_OS_WIN32
  Q_PROPERTY(QString winePrefix READ winePrefix WRITE winePrefix NOTIFY winePrefixChanged)
  Q_PROPERTY(QString bsProtonDir READ bsProtonDir WRITE bsProtonDir NOTIFY bsProtonDirChanged)
#endif
  Q_PROPERTY(QString bsInstall READ bsInstall WRITE bsInstall NOTIFY bsInstallChanged)
  Q_PROPERTY(QString bsInstall READ bsInstall WRITE bsInstall NOTIFY bsInstallChanged)

  Q_PROPERTY(QStringList gameVersionList)
  Q_PROPERTY(QStringList gameTypeList)

  /**
   * Qt Singals
   * TODO: These actually never get emitted, would only be needed if properties are changed from C++ and need to be updated in the GUI
   * For now the intention is it'll either be the command line or GUI in control, never mixed
   */
signals:
  void gameVersionChanged();
  void gameTypeChanged();
#ifndef Q_OS_WIN32
  void winePrefixChanged();
  void bsProtonDirChanged();
#endif
  void bsInstallChanged();

public:
  void setConfig(QString key, QString value);

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
  static const char* kGameVersion;
  static const char* kGameType;
  static const char* kWinePrefix;
  static const char* kBSProtonDir;
  static const char* kBSInstall;

  /**
   * Models for QML combo boxes
   * Boxes should be set to editable to allow
   * users to override if needed (Say if I'm late updating QBeat)
   */
  QStringList gameVersionList = {
    "1.3.0",
  };
  QStringList gameTypeList = {
    "steam",
    "oculus",
  };

  Settings();
  ~Settings();
private:
  void sanitisePath( QString& path );
};

#endif
