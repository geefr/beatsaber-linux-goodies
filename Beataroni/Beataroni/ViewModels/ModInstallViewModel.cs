using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Reactive;
using System.Threading;
using System.Threading.Tasks;
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

    private string installLogText = "";
    public string InstallLogText
    {
      get { return installLogText; }
      set {
        this.RaiseAndSetIfChanged(ref installLogText, value);
      }
    }

    private string currentMod = "";
    public string CurrentMod
    {
      get { return currentMod; }
      set { this.RaiseAndSetIfChanged(ref currentMod, value); }
    }

    private string currentStep = "";
    public string CurrentStep
    {
      get { return currentStep; }
      set { this.RaiseAndSetIfChanged(ref currentStep, value); }
    }

    public ModInstallViewModel()
    {
    }

    /// <summary>
    /// Perform mod installation actions
    /// Will consume the contents of ModsToInstall
    /// </summary>
    public void InstallMods(string bsInstall)
    {
      // TODO: For now just fire off the work as async, let the user decide if they want to quit halfway through the process
      // TODO: This should have a progress bar, loading spinner, etc
      Task.Run(() =>
      {
        ModInstaller installer = new ModInstaller();
        foreach ( var m in ModsToInstall )
        {
          CurrentMod = $"Current Mod: {m.mod.name}";

          if( m.selected )
          {
            CurrentStep = $"Current Step: Install";
            if( !installer.InstallMod(m.mod, bsInstall) )
            {
              installLogText += $"{m.mod.name}: Install failed\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
              continue;
            }
            else
            {
              installLogText += $"{m.mod.name}: Installed\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
            }

            CurrentStep = $"Current Step: Validate";
            if (!installer.ValidateMod(m.mod))
            {
              installLogText += $"{m.mod.name}: Validate failed (todo)\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
              // continue;
            }
            else
            {
              installLogText += $"{m.mod.name}: Validated\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
            }

            if ( m.mod.name.Equals("BSIPA") )
            {
              CurrentStep = $"Current Step: Patch Beat Saber";
              if (!installer.PatchBeatSaber(bsInstall))
              {
                installLogText += $"{m.mod.name}: Patching failed\n";
                this.RaisePropertyChanged(nameof(InstallLogText));
                continue;
              }
              else
              {
                installLogText += $"{m.mod.name}: Patching succeeded\n";
                this.RaisePropertyChanged(nameof(InstallLogText));
              }
            }
          }
          else
          {
            CurrentStep = $"Current Step: Uninstall";
            if( !installer.UninstallMod(m.mod) )
            {
              // TODO: This is fine - Any unchecked mods will be 'uninstalled'
              // but it's not implemented yet, commented out to avoid spamming
              // user with errors
              //installLogText += $"{m.mod.name}: Uninstall failed (todo)\n";
              //this.RaisePropertyChanged(nameof(InstallLogText));
            }
          }
        }

        CurrentMod = "SUCCESS: All Mods Installed";
        CurrentStep = "Please Check the log below, if all is good go and enjoy your mods :)";
      });
    }
  }
}
