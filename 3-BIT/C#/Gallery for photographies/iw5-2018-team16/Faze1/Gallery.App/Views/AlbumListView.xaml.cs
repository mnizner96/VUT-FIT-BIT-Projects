using Gallery.App.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Gallery.App.Views
{
    /// <summary>
    /// Interaction logic for AlbumListView.xaml
    /// </summary>
    public partial class AlbumListView : UserControl
    {
        public AlbumListView()
        {
            InitializeComponent();
            Loaded+= AlbumListView_Loaded;
        }

        private void AlbumListView_Loaded(object sender, System.Windows.RoutedEventArgs e)
        {
            if (DataContext is AlbumListViewModel viewModel)
            {
                viewModel.OnLoad();
            }
        }

        private void AddAlbumClick(object sender, RoutedEventArgs e)
        {
            AlbumInput albumDialog = new AlbumInput();
            albumDialog.ShowDialog();
        }
    }
}