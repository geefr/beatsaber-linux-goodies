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
  public class ModsViewModel : ViewModelBase
  {
    public class ModEntry
    {
      public Mod mod { get; set; }
      public bool selected { get; set; } = false;
    }

    private ObservableCollection<ModEntry> mods = null;
    public ObservableCollection<ModEntry> Mods
    {
      get { return mods; }
    }

    public ReactiveCommand<ModEntry, Unit> ModChecked { get; }

    public ReactiveCommand<Unit, Unit> ContinueButton { get; }
    public ReactiveCommand<Unit, Unit> SelectAllButton { get; }
    public ReactiveCommand<Unit, Unit> SelectNoneButton { get; }

    public ModsViewModel()
    {
      ModChecked = ReactiveCommand.Create<ModEntry>(x =>
      {
        // If a mod has been selected make sure all it's requirements are also selected
        // TODO: This action could be improved
        if( x.selected )
        {
          var deps = new HashSet<Mod>();
          Mod.GetDependencySet(x.mod, ref deps);

          foreach (var d in deps)
          {
            ModEntry entry = null;
            foreach (var m in Mods)
            {
              if (m.mod == d)
              {
                entry = m;
                break;
              }
            }
            if (entry != null)
            {
              // TODO: This little dance notifies the collection that something has changed, but it's not the best approach
              var index = Mods.IndexOf(entry);
              Mods.RemoveAt(index);
              entry.selected = true;
              Mods.Insert(index, entry);
            }
          }
        }
        // If a mod was deselected make sure anything that requires it is also deselected
        else
        {
          var dependents = new HashSet<Mod>();
          foreach( var m in Mods )
          {
            var deps = new HashSet<Mod>();
            Mod.GetDependencySet(m.mod, ref deps);
            if (deps.Contains(x.mod)) dependents.Add(m.mod);
          }

          foreach( var d in dependents )
          {
            ModEntry entry = null;
            foreach (var m in Mods)
            {
              if (m.mod == d)
              {
                entry = m;
                break;
              }
            }
            if (entry != null)
            {
              // TODO: This little dance notifies the collection that something has changed, but it's not the best approach
              var index = Mods.IndexOf(entry);
              Mods.RemoveAt(index);
              entry.selected = false;
              Mods.Insert(index, entry);
            }
          }
        }
      });

      ContinueButton = ReactiveCommand.Create(() => { });
      SelectAllButton = ReactiveCommand.Create(() =>
      {
        for( var i = 0; i < Mods.Count; ++i )
        {
          // TODO: This little dance notifies the collection that something has changed, but it's not the best approach
          var entry = Mods[i];
          Mods.RemoveAt(i);
          entry.selected = true;
          Mods.Insert(i, entry);
        }
      });
      SelectNoneButton = ReactiveCommand.Create(() =>
      {
        for (var i = 0; i < Mods.Count; ++i)
        {
          // TODO: This little dance notifies the collection that something has changed, but it's not the best approach
          var entry = Mods[i];
          Mods.RemoveAt(i);
          entry.selected = false;
          Mods.Insert(i, entry);
        }
      });
    }

    public void FetchMods(string gameVersion, string bsInstall)
    {
      // List all mods for the specified game version
      var filters = BeatModsV1.DefaultFilters;
      filters.Add("gameVersion", gameVersion);

      var m = BeatModsV1.FetchMods(BeatModsV1.DefaultFilters);
      var installer = new ModInstaller();
      var entries = new List<ModEntry>();
      foreach( var mm in m ) {
        var mod = new ModEntry() { mod = mm, selected = installer.IsModInstalled(mm, bsInstall) };
        entries.Add(mod);
      }

      mods = new ObservableCollection<ModEntry>(entries);
    }
  }
}
