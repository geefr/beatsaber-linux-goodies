#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>

#include <mods/mod.h>

class Actions : public QObject
{
  Q_OBJECT
public:
  explicit Actions(QObject *parent = nullptr);

  /// Check if the wine prefix is valid for running BSIPA
  Q_INVOKABLE bool isWinePrefixValid( QString winePrefix );

  /**
   * Setup a wine prefix in order to run BSIPA
   *
   * Only call this if isWinePrefixValid returned false, or the user explicitly chose to
   *
   * @note This may take a very long time. The user will need to follow the prompts in several setup wizards as .net is installed
   */
  Q_INVOKABLE bool setupWine( QString winePrefix );

  /**
   * Run actions needed to patch beat saber
   *
   * This is a linux-specific method, don't call it on windows ;)
   *
   */
  Q_INVOKABLE bool linuxModFix( QString bsInstall, QString protonInstall, QString winePrefix );

  /**
   * List all available mods
   *
   * TODO: Need regex/search filter
   * TODO: Will need to return list of mods, for now just logs to terminal
   */
  Q_INVOKABLE void listAvailableMods();


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
