using System;
using System.Reactive.Linq;
using ReactiveUI;
using Beataroni.Models.BeatMods;
using Beataroni.Services;
using System.Threading.Tasks;

namespace Beataroni.ViewModels
{
  public class MainWindowViewModel : ViewModelBase
  {
    /// The content of the view
    /// May be updated to any ViewModelBase,
    /// which will change to the other view
    /// In xaml set Content={Binding Content}
    ViewModelBase content;
    public ViewModelBase Content
    {
      get => content;
      // Show the new view, make a change notification to update the ui when changed
      private set => this.RaiseAndSetIfChanged(ref content, value);
    }

    private ModsViewModel ModList;
    private SettingsViewModel Settings;
    private ModInstallViewModel ModInstallView;

    public MainWindowViewModel(Settings settings)
    {
      ModList = new ModsViewModel();
      Settings = new SettingsViewModel(settings);
      ModInstallView = new ModInstallViewModel();
      Content = Settings;

      // Subscribe to events
      // Transition from settings view -> mods view
      Settings.ContinueButton.Subscribe(x =>
      {
        // TODO: This needs to be async
        ModList.FetchMods(Settings.BSVersion);
        Content = ModList;
      });

      // Transition from mods view -> mods install view
      ModList.ContinueButton.Subscribe(x =>
      {
        ModInstallView.ModsToInstall = new ModsViewModel.ModEntry[ModList.Mods.Count];
        ModList.Mods.CopyTo(ModInstallView.ModsToInstall, 0);
        Content = ModInstallView;
        ModInstallView.InstallMods();
      });
    }
  }
}
