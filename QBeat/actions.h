#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QTextStream>

#include <json/mod.h>

class Actions : public QObject
{
  Q_OBJECT
public:
  explicit Actions(QObject *parent = nullptr);

  /// Access the QBeat configuration
  void printAllConfig(QTextStream& qOut);
  void printConfig(QTextStream& qOut, QString key);
  void setConfig(QString key, QString val);

#ifndef Q_OS_WIN32
  /// Check if the wine prefix is valid for running BSIPA
  Q_INVOKABLE bool isWinePrefixValid();

  /**
   * Setup a wine prefix in order to run BSIPA
   *
   * Only call this if isWinePrefixValid returned false, or the user explicitly chose to
   *
   * @note This may take a very long time. The user will need to follow the prompts in several setup wizards as .net is installed
   */
  Q_INVOKABLE bool setupWine();

  /**
   * Run actions needed to patch beat saber
   *
   * This is a linux-specific method, don't call it on windows ;)
   *
   */
  Q_INVOKABLE bool patchBeatSaber();
#else
# error "Actions::patchBeatSaber not implemented for Windows"
#endif

  /**
   * List all available mods
   *
   * TODO: Need regex/search filter
   * TODO: Will need to return list of mods, for now just logs to terminal
   */
  Q_INVOKABLE std::list<Mod> listAvailableMods();

  /**
   * Fetch a mod by name
   */
  Q_INVOKABLE Mod getNamedMod( QString name );

  /**
   * Install a mod to the specified directory
   */
  Q_INVOKABLE bool installMod( Mod mod, bool includeDependencies = true );
  Q_INVOKABLE bool installAllMods();

  /**
   * Download a mod to the specified directory
   */
  Q_INVOKABLE bool downloadMod( Mod mod, QString directory, bool includeDependencies = true );

  /**
   * Validate a mod
   */
  Q_INVOKABLE bool validateMod( Mod mod, bool includeDependencies = true );

  /**
   * @brief Filter a list of mods to a specific game version
   * @param mods List of mods to filter
   * @param version Game version to filter to
   * @return Sub-set of mods which match gameVersion
   */
  std::list<Mod> filterModsToVersion(std::list<Mod> mods, QString version);

signals:

public slots:
};

#endif // ACTIONS_H
