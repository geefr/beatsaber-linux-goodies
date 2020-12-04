using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Beataroni.ViewModels;
using Beataroni.Views;
using Beataroni.Services;

namespace Beataroni
{
    public class App : Application
    {
        public Settings Config {get;} = new Settings();

        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);
        }

        public override void OnFrameworkInitializationCompleted()
        {
            if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                desktop.MainWindow = new MainWindow
                {
                    DataContext = new MainWindowViewModel(Config),
                };
            }

            base.OnFrameworkInitializationCompleted();
        }
    }
}