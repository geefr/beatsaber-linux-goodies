using System.Collections.Generic;
using System.Collections.ObjectModel;
using Beataroni.Models.BeatMods;
using Beataroni.Services;

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

      //public string ModName
      //{
      //  get{ return mod.name; }
      //}
    }

    public ModsViewModel()
    {
    }

    public void FetchMods(string gameVersion)
    {
      // TODO: Need filters from somewhere - This shouldn't be called until version/etc has been selected?
      var filters = BeatModsV1.DefaultFilters;
      filters.Add("gameVersion", gameVersion);

      var m = BeatModsV1.FetchMods(BeatModsV1.DefaultFilters);

      var entries = new List<ModEntry>();
      foreach( var mm in m ) entries.Add(new ModEntry() { mod = mm });

      mods = new ObservableCollection<ModEntry>(entries);
    }

    private ObservableCollection<ModEntry> mods = null;
    public ObservableCollection<ModEntry> Mods
    {
      get { return mods; }
    }
  }
}
