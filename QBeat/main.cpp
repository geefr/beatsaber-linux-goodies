#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>

#include "actions.h"
#include "util.h"
#include "settings.h"
#include "json/mod.h"

int main(int argc, char *argv[])
{
  // TODO: This suppresses a debug log whenever QBeat is run on my system, not sure why it exists
  qputenv("QT_LOGGING_RULES", "qt5ct.debug=false");


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
  // TODO: Some kind of config backup system - does the mod api list which files are considered 'config' or just copy the whole userdata folder?
  // TODO: Support backdoor/injection via 2nd api - disabled by default but like a directory of json files or somesuch, to allow handling unapproved mods at user's risk


  // In the future should use application storage to cache this, similar style to git config
  parser.addPositionalArgument("arg1", "First argument to action");
  parser.addPositionalArgument("arg2", "Second argument to action");
  parser.addPositionalArgument("arg3", "Third argument to action");
  parser.addPositionalArgument("arg4", "Fourth argument to action");

  QCommandLineOption actionConfig = {"config", "Get/Set QBeat configuration variables"};
#ifndef Q_OS_WIN32
  QCommandLineOption actionValidateWine = {"validate-wine", "Validate wine installation"};
  QCommandLineOption actionSetupWine = {"setup-wine", "Setup a wine installation for BSIPA"};
#endif
  QCommandLineOption actionPatch = {"patch", "Patch game with BSIPA"};
  QCommandLineOption actionList = {"list", "List available mods"};
#ifdef Q_DEBUG
  QCommandLineOption actionDownload = {"download", "(Debug builds only) Download a mod but don't install it"};
#endif
  QCommandLineOption actionInstall = {"install", "Install a mod"};
  QCommandLineOption actionInstallEverything = {"install-all", "Install every mod (WARNING: This is probably a bad idea :O)"};
  QCommandLineOption actionValidate = {"validate", "Validate that a mod is installed correctly"};
  QCommandLineOption actionRemove = {"remove", "Uninstall a mod"};
  QCommandLineOption actionGUI = {"GUI", "Start the GUI (Incomplete)"};

  /*
  QCommandLineOption actionListInstalled = {"list-installed", "List installed mods"};
  QCommandLineOption actionUpdateInstalled = {"update", "Update installed mods"};
  */
  parser.addOptions({
    actionConfig,
#ifndef Q_OS_WIN32
    actionValidateWine,
    actionSetupWine,
#endif
    actionPatch,
    actionList,
#ifdef Q_DEBUG
    actionDownload,
#endif
    actionInstall,
    actionValidate,
    actionRemove,
    actionInstallEverything,
    actionGUI,
  /*
    actionListInstalled,
    actionUpdateInstalled,
                        */
  });

  parser.process(app);

  // The possible actions this application can perform
  // TODO: This should be in some other class, it's way too big and messy for main
  Actions actions;
  QTextStream qOut( stdout );

  if( parser.isSet(actionConfig) )
  {
    if( parser.positionalArguments().size() < 1 ) {
      qOut << "Get all variables  : --config get\n"
              "Get named variable : --config get <variable name>\n"
              "Set named variable : --config set <variable name> <new value>\n";
      return EXIT_SUCCESS;
    }
    auto configMode = parser.positionalArguments()[0];
    if( configMode == "get" && parser.positionalArguments().size() == 1) {
        actions.printAllConfig(qOut);
        return EXIT_SUCCESS;
    } else if( configMode == "get" && parser.positionalArguments().size() == 2 ) {
        actions.printConfig(qOut, parser.positionalArguments()[1]);
        return EXIT_SUCCESS;
    } else if( configMode == "set" && parser.positionalArguments().size() == 3) {
        Settings::instance.setConfig(parser.positionalArguments()[1], parser.positionalArguments()[2]);
        return EXIT_SUCCESS;
    } else {
      qOut << "Get all variables  : --config get\n"
              "Get named variable : --config get <variable name>\n"
              "Set named variable : --config set <variable name> <new value>\n";
      return EXIT_FAILURE;
    }
  }
#ifndef Q_OS_WIN32
  if( parser.isSet(actionValidateWine ))
  {
    if( Settings::instance.winePrefix().isEmpty() ) {
      qOut << "ERROR: Wine prefix not set, run QBeat --config set " << Settings::kWinePrefix << " <prefix> to configure\n";
      return EXIT_FAILURE;
    }
    if( actions.isWinePrefixValid() ) {
      qOut << "SUCCESS: Wine prefix at " + Settings::instance.winePrefix() + " seem valid\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "FAILURE: Wine prefix at " + Settings::instance.winePrefix() + " does not seem valid\n";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionSetupWine))
  {
    if( Settings::instance.winePrefix().isEmpty() ) {
      qOut << "ERROR: Wine prefix not set, run QBeat --config set " << Settings::kWinePrefix << " <prefix> to configure\n";
      return EXIT_FAILURE;
    }
    qOut << "Running wine setup script. Please be patient and choose the default option in all of the setup wizards. A status message will be printed once setup is complete\n";
    qOut.flush();
    if( actions.setupWine() ) {
      qOut << "SUCCESS: Wine prefix setup for running BSIPA\n";
    } else {
      qOut << "FAILURE: Setup of wine prefix failed\n";
    }
  }
#endif
  else if( parser.isSet(actionPatch))
  {
#ifndef Q_OS_WIN32
    if( Settings::instance.winePrefix().isEmpty() ) {
      qOut << "ERROR: Wine prefix not set, run QBeat --config set " << Settings::kWinePrefix << " <prefix> to configure\n";
      return EXIT_FAILURE;
    }
    if( Settings::instance.bsProtonDir().isEmpty() ) {
      qOut << "ERROR: Beat Saber Proton dir not set, run QBeat --config set " << Settings::kBSProtonDir << " <dir> to configure\n";
      return EXIT_FAILURE;
    }
#endif
    if( Settings::instance.bsInstall().isEmpty() ) {
      qOut << "ERROR: Beat Saber directory not set, run QBeat --config set " << Settings::kBSInstall << " <dir> to configure\n";
      return EXIT_FAILURE;
    }

    qOut << "Patching game with BSIPA. Please be patient. A status message will be printed once setup is complete\n";
    if( actions.patchBeatSaber() ) {
      qOut << "SUCCESS: Game patched with BSIPA\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "FAILURE: Patching game failed\n";
      return EXIT_FAILURE;
    }
  }

#ifdef Q_DEBUG
  else if( parser.isSet(actionDownload) )
  {
    if( parser.positionalArguments().size() < 2 ) {
      qOut << "USAGE: --download <mod name> <Destination Directory>\n";
      return EXIT_FAILURE;
    }

    qOut << "Downloading mod...\n";
    auto mod = actions.getNamedMod(parser.positionalArguments()[0]);
    if( mod.mID.size() == 0 ) // TODO: This is nasty
    {
      qOut << "ERROR: Unable to find mod named: " + parser.positionalArguments()[0];
      return EXIT_FAILURE;
    }

    if( actions.downloadMod( mod, parser.positionalArguments()[1] ) )
    {
      qOut << "SUCCESS: Mod downloaded to: " + parser.positionalArguments()[1];
      return EXIT_SUCCESS;
    } else {
      qOut << "ERROR: Failed to download mod: ";
      return EXIT_FAILURE;
    }
  }
#endif
  else if( parser.isSet(actionInstall) )
  {
    if( Settings::instance.bsInstall().isEmpty() ) {
      qOut << "ERROR: Beat Saber directory not set, run QBeat --config set " << Settings::kBSInstall << " <dir> to configure\n";
      return EXIT_FAILURE;
    }
    if( parser.positionalArguments().size() < 1 ) {
      qOut << "USAGE: --install <mod name>\n";
      return EXIT_FAILURE;
    }
    auto modName = parser.positionalArguments()[0];
    qOut << "Installing mod: " << modName << "\n";
    auto mod = actions.getNamedMod(modName);
    if( mod.mID.size() == 0 ) // TODO: This is nasty
    {
      qOut << "ERROR: Unable to find mod named: " << modName << "\n";
      return EXIT_FAILURE;
    }

    if( actions.installMod( mod ) )
    {
      qOut << "SUCCESS: Mod installed: " << modName << "\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "ERROR: Failed to install mod: " << modName << "\n";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionValidate) )
  {
    if( Settings::instance.bsInstall().isEmpty() ) {
      qOut << "ERROR: Beat Saber directory not set, run QBeat --config set " << Settings::kBSInstall << " <dir> to configure\n";
      return EXIT_FAILURE;
    }
    if( parser.positionalArguments().size() < 1 ) {
      qOut << "USAGE: --validate <mod name>\n";
      return EXIT_FAILURE;
    }
    auto modName = parser.positionalArguments()[0];
    qOut << "Validating mod: " << modName << "\n";
    auto mod = actions.getNamedMod(modName);
    if( mod.mID.size() == 0 ) // TODO: This is nasty
    {
      qOut << "ERROR: Unable to find mod named: " << modName << "\n";
      return EXIT_FAILURE;
    }

    if( actions.validateMod( mod ) )
    {
      qOut << "SUCCESS: Mod valid: " << modName << "\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "ERROR: Mod not valid, run QBeat --install \"" << modName << "\" to fix \n";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionRemove) )
  {
    if( Settings::instance.bsInstall().isEmpty() ) {
      qOut << "ERROR: Beat Saber directory not set, run QBeat --config set " << Settings::kBSInstall << " <dir> to configure\n";
      return EXIT_FAILURE;
    }
    if( parser.positionalArguments().size() < 1 ) {
      qOut << "USAGE: --remove <mod name>\n";
      return EXIT_FAILURE;
    }
    auto modName = parser.positionalArguments()[0];
    qOut << "Removing mod: " << modName << "\n";
    auto mod = actions.getNamedMod(modName);
    if( mod.mID.size() == 0 ) // TODO: This is nasty
    {
      qOut << "ERROR: Unable to find mod named: " << modName << "\n";
      return EXIT_FAILURE;
    }

    if( actions.removeMod( mod ) )
    {
      qOut << "SUCCESS: Mod removed: " << modName << "\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "ERROR: Failed to remove mod: " << modName << "\n";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionInstallEverything) )
  {
    if( Settings::instance.bsInstall().isEmpty() ) {
      qOut << "ERROR: Beat Saber directory not set, run QBeat --config set " << Settings::kBSInstall << " <dir> to configure\n";
      return EXIT_FAILURE;
    }

    if( actions.installAllMods() )
    {
      qOut << "SUCCESS: Everything has been installed!\n";
      return EXIT_SUCCESS;
    } else {
      qOut << "ERROR: Failed to install some mod, check the log\n";
      return EXIT_FAILURE;
    }
  }
  else if( parser.isSet(actionList))
  {
    auto mods = actions.listAvailableMods();
    for( auto mod : mods ) {
      qOut << mod.mName << "\n";
    }
    return EXIT_SUCCESS;
  }
  else if( parser.isSet(actionGUI) )
  {
    // If nothing specific was requested on the command line start the GUI
    QQmlApplicationEngine engine;

    qmlRegisterType<Settings>("uk.co.gfrancisdev.qbeat.settings", 1, 0, "Settings");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
  }
}
