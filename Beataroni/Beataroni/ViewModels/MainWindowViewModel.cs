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

    public ModsViewModel ModList { get; }
    public SettingsViewModel Settings { get; }

    public MainWindowViewModel(Settings settings)
    {
      ModList = new ModsViewModel();
      Settings = new SettingsViewModel(settings);
      Content = Settings;

      // Subscribe to events
      Settings.ContinueButton.Subscribe(x =>
      {
        // TODO: This needs to be async
        ModList.FetchMods();
        Content = ModList;
      });
    }
  }
}
