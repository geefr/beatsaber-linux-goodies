using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Beataroni.Views
{
  public class ModInstallView : UserControl
  {
    public ModInstallView()
    {
      this.InitializeComponent();
    }

    private void InitializeComponent()
    {
      AvaloniaXamlLoader.Load(this);
    }
  }
}
