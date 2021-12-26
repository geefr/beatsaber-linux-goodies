using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Reactive;
using System.Reactive.Linq;
using System.Threading.Tasks;
using Beataroni.Models.BeatMods;
using Beataroni.Services;
using ReactiveUI;

namespace Beataroni.ViewModels
{
  /*
   * Adapter class/view model between mod list and Observable collection
   */
  public class SettingsViewModel : ViewModelBase
  {
    private ObservableCollection<string> bsVersions;
    private string bsVersionTextLabel;
    private Settings settings;
    private int bsVersionIndex = 0;

    public string BSInstall
    {
      get { return settings.BSInstall; }
      set
      {
        if (value != settings.BSInstall)
        {
          settings.BSInstall = value;
          this.RaisePropertyChanged(nameof(BSInstall));
        }
      }
    }

    public ObservableCollection<string> BSVersions { 
      get { return bsVersions; }
    }

    public string BSVersion {
      get { return settings.BSVersion; }
      set
      {
        if (value != settings.BSVersion)
        {
          settings.BSVersion = value;
          this.RaisePropertyChanged(nameof(BSVersion));
        }
      }
    }

    public string BSVersionTextLabel {
      get { return bsVersionTextLabel; }
      set { this.RaiseAndSetIfChanged(ref bsVersionTextLabel, value); }
    }

    public int BSVersionIndex {
      get { return bsVersionIndex; }
      set {
        if (value != 0)
        {
          BSVersionTextLabel = "Beatsaber Version (not latest)";
        }
        else
        {
          BSVersionTextLabel = "Beatsaber Version";
        }
        this.RaiseAndSetIfChanged(ref bsVersionIndex, value);
      }
    }

    public ReactiveCommand<Unit, Unit> ContinueButton { get; }

    public SettingsViewModel()
    {
      settings = new Settings();
    }

    public SettingsViewModel(Settings s)
    {
      settings = s;

      // Fetch data from beatmods api, setup defaults
      //Task.Run(() =>
      //{
      // TODO: This should almost certainly be async (But it's a small request) Assuming a normal threading model in avalonia
      var versions = BeatModsV1.FetchBSVersions();
      var versionSet = new ObservableCollection<string>(versions);
      this.RaiseAndSetIfChanged(ref bsVersions, versionSet);

      if (!string.IsNullOrWhiteSpace(settings.BSVersion))
      {
        BSVersionIndex = versions.IndexOf(settings.BSVersion);
      }
      else
      {
        BSVersionIndex = 0;
      }

      // Criteria for continue button to be enabled
      var bsInstallValid = this.WhenAnyValue(
        x => x.BSInstall,
        x => SettingsViewModel.IsBSInstallValid(x)
      );
      // var bsVersionValid = this.WhenAnyValue(
      //   x => x.BSVersion,
      //   x => !string.IsNullOrEmpty(x)
      // );
      // TODO: This doesn't work because the When triggers when both are observable
      // i.e when both the settings have been changed.
      // Disabled for now, as we'll always have a version selected, and only really need to
      // validate the install path..Will need to re-enable later on when things are more complex
      // var settingsValid = Observable.When(
      //   bsInstallValid.And(bsVersionValid)
      //   .Then((x1,x2) => x1 & x2));
      // Command for button. Enabled if validation passes, fires the command when clicked.
      // Subscribed to in MainWindowViewModel
      ContinueButton = ReactiveCommand.Create(
      () => { },
      bsInstallValid);
    }

    private static bool IsBSInstallValid(string path)
    {
      if( string.IsNullOrEmpty(path) ) return false;
      if( !Directory.Exists(path) ) return false;
      if( !File.Exists($"{path}/Beat Saber.exe")) return false;
      return true;
    }
  }
}
