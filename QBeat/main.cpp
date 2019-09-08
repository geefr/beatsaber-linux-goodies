#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QDebug>

#include "actions.h"
#include "util.h"
#include "settings.h"

int main(int argc, char *argv[])
{
  /// Check for environment overrides and debug options
  Settings::initSettings();

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  /*
   * Handle command line arguments
   * For initial testing, and for those that want it command line options
   * for the each of the applicatrion's actions
   * GUI will call through to the same underlying code
   */
  QCommandLineParser parser;
  parser.setApplicationDescription("QBeat: Cross Platform mod installer for Beat Saber");
  parser.addHelpOption();

  // TODO: For now must be specified on the command line each time
  // In the future should use application storage to cache this, similar style to git config
  parser.addPositionalArgument("arg1", "First argument to action");
  parser.addPositionalArgument("arg2", "Second argument to action");
  parser.addPositionalArgument("arg3", "Third argument to action");

  QCommandLineOption actionValidateWine = {"validate-wine", "Validate wine installation"};
  QCommandLineOption actionSetupWine = {"setup-wine", "Setup a wine installation for BSIPA"};
  QCommandLineOption actionLinuxModFix = {"linux-patch", "Patch game with BSIPA"};
  /*
  QCommandLineOption actionValidate = {"validate", "Validate Beat Saber/Mod Installation"};
  QCommandLineOption actionList = {"list", "List available mods"};
  QCommandLineOption actionListInstalled = {"list-installed", "List installed mods"};
  QCommandLineOption actionUpdateInstalled = {"update", "Update installed mods"};
  */
  parser.addOptions({
    actionValidateWine,
    actionSetupWine,
    actionLinuxModFix
  /*
    actionValidate,
    actionList,
    actionListInstalled,
    actionUpdateInstalled,
                        */
  });

  parser.process(app);

  /*
  // Fetch the action to perform
  if( parser.positionalArguments().size() < 1 )
    qDebug() << "ERROR: Argument 1 not specified";
  }
  auto bsDir = parser.positionalArguments()[0];
  {
*/
  /*
#ifndef Q_WS_WIN
  if( parser.positionalArguments().size() < 2 )
  {
    // TODO: Only for installation validation?
    qDebug() << "ERROR: Proton directory not specified";
    std::exit( EXIT_FAILURE );
  }
  auto protonDir = parser.positionalArguments()[1];
#endif
*/

  // The possible actions this application can perform
  Actions actions;

  if( parser.isSet(actionValidateWine ))
  {
    if( parser.positionalArguments().size() < 1 ) {
      qDebug() << "USAGE: --validate-wine <wine prefix>";
      return EXIT_FAILURE;
    }
    if( actions.isWinePrefixValid( parser.positionalArguments()[0] ) ) {
      qDebug() << "SUCCESS: Wine prefix at " + parser.positionalArguments()[0] + " seem valid";
      return EXIT_SUCCESS;
    } else {
      qDebug() << "FAILURE: Wine prefix at " + parser.positionalArguments()[0] + " does not seem valid";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionSetupWine))
  {
    if( parser.positionalArguments().size() < 1 ) {
      qDebug() << "USAGE: --setup-wine <wine prefix>";
      return EXIT_FAILURE;
    }
    qDebug() << "Running wine setup script. Please be patient and choose the default option in all of the setup wizards. A status message will be printed once setup is complete";
    if( actions.setupWine( parser.positionalArguments()[0] ) ) {
      qDebug() << "SUCCESS: Wine prefix setup for running BSIPA";
    } else {
      qDebug() << "FAILURE: Setup of wine prefix failed";
    }
  }
  else if( parser.isSet(actionLinuxModFix))
  {
    if( parser.positionalArguments().size() < 2 ) {
      qDebug() << "USAGE: --linux-patch <Beat Saber Install> <Proton Install> [Wine Prefix (Optional)]";
      return EXIT_FAILURE;
    }

    qDebug() << "Patching game with BSIPA. Please be patient. A status message will be printed once setup is complete";
    if( actions.linuxModFix( parser.positionalArguments()[0], parser.positionalArguments()[1],
                             parser.positionalArguments().size() >= 3 ? parser.positionalArguments()[2] : "" ) ) {
      qDebug() << "SUCCESS: Game patched with BSIPA";
    } else {
      qDebug() << "FAILURE: Patching game failed";
    }
  }
  else {
    // Fall back to --help and exit
    parser.showHelp();
  }



// TODO: GUI support will come after command line ;)
#if 0
  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  engine.load(url);


  return app.exec();
#endif
  return EXIT_SUCCESS;
}
