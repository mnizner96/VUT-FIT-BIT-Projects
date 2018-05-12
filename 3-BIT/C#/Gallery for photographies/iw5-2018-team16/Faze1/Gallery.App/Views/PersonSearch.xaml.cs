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
    /// Interaction logic for ResolFilterOrSearch.xaml
    /// </summary>
    public partial class PersonSearch : Window
    {
        public PersonSearch(string question, string defaultAnswer = "")
        {
            InitializeComponent();
            lblQuestion.Content = question;
            txtAnswer.Text = defaultAnswer;

        }
        private void SearchOk_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
        private void Window_ContentRendered(object sender, EventArgs e)
        {
            txtAnswer.SelectAll();
            txtAnswer.Focus();
        }
    }
}