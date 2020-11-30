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

        public MainWindowViewModel(Settings settings)
        {
            ModList = new ModsViewModel();
            Task.Run(() => {
                ModList.FetchMods();
                Content = ModList;
            });
            
            
        }

        // public void AddNewItem()
        // {
        //     // Display the AdditemView
        //     var ni = new AddItemViewModel();

        //     // Add the TodoItem to the model
        //     // First merge ni's Observables into one stream
        //     // - ReactiveCommands Ok and Cancel are Observables, produce a value each time command is executed
        //     // - Ok produces a TodoItem, Cancel doesn't, so map to the correct type here
        //     Observable.Merge(
        //         ni.Ok,
        //         ni.Cancel.Select(_ => (TodoItem)null))
        //     // Then take the first one (First click of ok or cancel buttons)
        //     .Take(1)
        //     // Subscribe to the result of this sequence
        //     // NOTE! Make sure you're 'using System;' in the file, otherwise this doesn't work. Subscribe is an extension method within that namespace (But of course C# compiler doesn't help us out there >:[)
        //     .Subscribe(x =>
        //     {
        //         if( x != null ) TodoList.Items.Add(x);

        //         // Item added/cancelled, return view to the TodoList
        //         Content = TodoList;
        //     });

        //     Content = ni;
        // }

    }
}
