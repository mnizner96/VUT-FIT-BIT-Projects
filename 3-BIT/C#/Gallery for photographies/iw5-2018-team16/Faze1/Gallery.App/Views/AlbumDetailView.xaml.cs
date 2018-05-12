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
    /// Interaction logic for AlbumDetailView.xaml
    /// </summary>
    public partial class AlbumDetailView : UserControl
    {
        public AlbumDetailView()
        {
            InitializeComponent();
            Loaded += AlbumDetailView_Loaded;
        }
        private void AlbumDetailView_Loaded(object sender, System.Windows.RoutedEventArgs e)
        {
            if (DataContext is AlbumDetailViewModel viewModel)
            {
                viewModel.OnLoad();
            }
        }

        private void DateFilter_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            DateFilter dfDialog = new DateFilter();
            dfDialog.ShowDialog();

        }

        private void FormatFilter_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            FormatFilter ffDialog = new FormatFilter();
            ffDialog.ShowDialog();
        }
        private void ResolutionFilter_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            ResolutionFilter rfDialog = new ResolutionFilter("0");
            rfDialog.ShowDialog();
            // = rfDialog.Answer;
        }
        private void Search_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            Search sDialog = new Search("Write the name of the photo","");
            sDialog.ShowDialog();
            // = rfDialog.Answer;
        }

        private void ChangeName_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            ChangeName cnDialog = new ChangeName("Write the new name", "");
            cnDialog.Title = "Change Album Name";
            cnDialog.ShowDialog();
            // = rfDialog.Answer;
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
