using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reactive;
using Avalonia.Interactivity;
using Beataroni.Models.BeatMods;
using Beataroni.Services;
using ReactiveUI;

namespace Beataroni.ViewModels
{
  /*
   * Adapter class/view model between mod list and Observable collection
   */
  public class ModInstallViewModel : ViewModelBase
  {
    public ModsViewModel.ModEntry[] ModsToInstall { get; set; } = null;

    public ModInstallViewModel()
    {
    }
  }
}
