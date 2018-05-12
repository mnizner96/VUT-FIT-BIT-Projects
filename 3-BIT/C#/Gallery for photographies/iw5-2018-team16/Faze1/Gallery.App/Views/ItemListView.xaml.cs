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
    public partial class ItemListView : UserControl
    {
        public ItemListView()
        {
            InitializeComponent();
            Loaded += ItemListView_Loaded;
        }
        private void ItemListView_Loaded(object sender, RoutedEventArgs e)
        {
            if (DataContext is ItemListViewModel viewModel)
            {
                viewModel.OnLoad();
            }
        }
        private void Search_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            ItemSearch rfDialog = new ItemSearch("Write the name of the item", "");
            rfDialog.Title = "Search Item";
            rfDialog.ShowDialog();
            // = rfDialog.Answer;
        }
    }

}
