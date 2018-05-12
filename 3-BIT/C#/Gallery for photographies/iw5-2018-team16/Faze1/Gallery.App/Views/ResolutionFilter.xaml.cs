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
using System.Windows.Shapes;

namespace Gallery.App.Views
{
    /// <summary>
    /// Interaction logic for ResolutionFilter.xaml
    /// </summary>
    public partial class ResolutionFilter : Window
    {
        public ResolutionFilter(string defaultAnswer = "0")
        {
            InitializeComponent();
            txtAnswer.Text = defaultAnswer;
            txtAnswer2.Text = defaultAnswer;
        }
        private void ResolutionOk_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }


    }

}
