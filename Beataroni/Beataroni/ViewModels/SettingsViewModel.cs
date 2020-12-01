using System.Collections.Generic;
using System.Collections.ObjectModel;
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

    public int BSVersionIndex {
      get { return bsVersionIndex; }
      set { this.RaiseAndSetIfChanged(ref bsVersionIndex, value); }
    }

    public ReactiveCommand<Unit, Unit> ContinueButton { get; }

    public SettingsViewModel(Settings s)
    {
      settings = s;
      // TODO: Setup properties to pass through to Services.Settings

      // Fetch data from beatmods api, setup defaults
      //Task.Run(() =>
      //{
      // TODO: This should almost certainly be async (But it's a small request) Assuming a normal threading model in avalonia
      var versions = BeatModsV1.FetchBSVersions();
      var versionSet = new ObservableCollection<string>(versions);
      this.RaiseAndSetIfChanged(ref bsVersions, versionSet);
      // TODO: Should be versions.IndexOf(settings.BSVersion) to select whatever settings says (Or read game version vs last-installed version, modassistant has that so it's clearly possible)
      BSVersionIndex = 0;
      //});

      // Criteria for continue button to be enabled
      var bsInstallValid = this.WhenAnyValue(
        x => x.BSInstall,
        x => !string.IsNullOrEmpty(x)
      );
      var bsVersionValid = this.WhenAnyValue(
        x => x.BSVersion,
        x => !string.IsNullOrEmpty(x)
      );
      var settingsValid = Observable.When(
        bsInstallValid.And(bsVersionValid)
        .Then((x1,x2) => x1 && x2));
      // Command for button. Enabled if validation passes, fires the command when clicked.
      // Subscribed to in MainWindowViewModel
      ContinueButton = ReactiveCommand.Create(
      () => { },
      settingsValid);
    }
  }
}
