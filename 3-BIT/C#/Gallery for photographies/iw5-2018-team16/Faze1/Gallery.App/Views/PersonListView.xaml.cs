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
    /// Interaction logic for PersonListView.xaml
    /// </summary>
    public partial class PersonListView : UserControl
    {
        public PersonListView()
        {
            InitializeComponent();
            Loaded += PersonListView_Loaded;
        }
        private void PersonListView_Loaded(object sender, RoutedEventArgs e)
        {
            if (DataContext is PersonListViewModel viewModel)
            {
                viewModel.OnLoad();
            }
        }
        private void Search_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            PersonSearch rfDialog = new PersonSearch("Write the name of the person","");
            rfDialog.Title = "Search Person"; 
            rfDialog.ShowDialog();
            // = rfDialog.Answer;
        }
    }

}
