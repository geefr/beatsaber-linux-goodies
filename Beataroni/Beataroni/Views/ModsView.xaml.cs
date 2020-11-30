using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Beataroni.Views
{
    public class ModsView : UserControl
    {
        public ModsView()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            AvaloniaXamlLoader.Load(this);
        }
    }
}