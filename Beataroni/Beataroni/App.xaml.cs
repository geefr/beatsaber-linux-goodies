using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Beataroni.ViewModels;
using Beataroni.Views;
using Beataroni.Services;
using System;

namespace Beataroni
{
  public class App : Application
  {
    public Settings Config { get; } = Settings.Load();

    public override void Initialize()
    {
      AppDomain.CurrentDomain.ProcessExit += new EventHandler(this.OnProcessExit);
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

    void OnProcessExit(object sender, EventArgs e)
    {
      Config.Save();
    }
  }
}