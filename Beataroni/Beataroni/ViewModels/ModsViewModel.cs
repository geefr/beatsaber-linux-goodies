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
        public ModsViewModel()
        {
        }

        public void FetchMods()
        {
            // TODO: Need filters from somewhere - This shouldn't be called until version/etc has been selected?
            var m = BeatModsV1.FetchMods(BeatModsV1.DefaultFilters);
            mods = new ObservableCollection<Mod>(m);
        }

        private ObservableCollection<Mod> mods = null;
        public ObservableCollection<Mod> Mods {
            get{return mods;}
        }
    }
}
