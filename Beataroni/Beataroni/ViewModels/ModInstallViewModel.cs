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

    public void LogLine(string msg)
    {
      installLogText += $"{msg}\n";
      this.RaisePropertyChanged(nameof(InstallLogText));
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
            if( !installer.InstallMod(m.mod, bsInstall, LogLine) )
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
            if (!installer.ValidateMod(m.mod, bsInstall, LogLine))
            {
              installLogText += $"{m.mod.name}: Validate failed (todo)\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
              continue;
            }
            else
            {
              installLogText += $"{m.mod.name}: Validated\n";
              this.RaisePropertyChanged(nameof(InstallLogText));
            }

            if ( m.mod.name.Equals("BSIPA") )
            {
              CurrentStep = $"Current Step: Patch Beat Saber";
              if (!installer.PatchBeatSaber(bsInstall, LogLine))
              {
                // If this setup step has failed then no mods will work in the game
                // The most likely reason is the proton prefix is missing
                // Which will happen if the game hasn't been run first
                // before the mods were installed
                installLogText += $"{m.mod.name}: Patching failed\n";

                installLogText += $"\n\nSorry, looks like patching failed\n";
                installLogText += $"Your installed mods will not work\n";
                installLogText += $"First try a fresh install (Run the game once before Beataroni)\n";
                installLogText += $"Then if it still fails please raise a bug at:\n";
                installLogText += $"https://github.com/geefr/beatsaber-linux-goodies\n\n";

                CurrentMod = "ERROR: Setup Failed";
                CurrentStep = "";

                this.RaisePropertyChanged(nameof(InstallLogText));
                return;
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
            // TODO: At the moment we don't log anything here to avoid user-spam
            // Do they really care if a mod was uninstalled? Really it's just a pass
            // over the list to ensure what's installed matches the user's selection
            installer.UninstallMod(m.mod, bsInstall, LogLine);
          }
        }

        CurrentMod = "SUCCESS: Setup Completed";
        CurrentStep = "Please Check the log below, if all is good go and enjoy your mods :)";
      });
    }
  }
}
