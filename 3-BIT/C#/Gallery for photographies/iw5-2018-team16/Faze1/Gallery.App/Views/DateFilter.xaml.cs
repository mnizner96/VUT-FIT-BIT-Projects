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
    /// Interaction logic for DateFilter.xaml
    /// </summary>
    public partial class DateFilter : Window
    {
        public DateFilter()
        {
            InitializeComponent();
            
            DatePk.SelectedDate = DateTime.Today;
        }
        private void DatePk_DateChanged(object sender,
            SelectionChangedEventArgs e)
        {
            var picker = sender as DatePicker;
            DateTime? date = picker.SelectedDate;
            if (date == null)
            {
                DatePk.Text = DateTime.Today.ToShortDateString();
            }
            else
            {
                DatePk.Text = date.Value.ToShortDateString();
            }
        }

        private void DateOk_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
             
        }

    }
}

