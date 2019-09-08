#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>

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

signals:

public slots:
};

#endif // ACTIONS_H
