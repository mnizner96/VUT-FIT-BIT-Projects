using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
    /// Interaction logic for PhotographyDetailView.xaml
    /// </summary>
    public partial class PhotographyDetailView : UserControl
    {
        bool mouseDown = false; // Set to 'true' when mouse is held down.
        Point mouseDownPos; // The point where the mouse button was clicked down
        public PhotographyDetailView()
        {
            InitializeComponent();
            TagBoxItem.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonName.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonSureName.Visibility = System.Windows.Visibility.Hidden;
            TagButtonItem.Visibility = System.Windows.Visibility.Hidden;
            TagButtonPerson.Visibility = System.Windows.Visibility.Hidden;
            CancelButton.Visibility = System.Windows.Visibility.Hidden;
            OkItem.Visibility = System.Windows.Visibility.Hidden;
            OkPersonFirstName.Visibility = System.Windows.Visibility.Hidden;
            OkPerson.Visibility = System.Windows.Visibility.Hidden;


        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
        }

        private void Button_loadClick(object sender, RoutedEventArgs e)
        {


            OpenFileDialog op = new OpenFileDialog();
            op.Title = "Select a picture";
            op.Filter = "All supported graphics|*.jpg;*.jpeg;*.png|" +
              "JPEG (.jpg;.jpeg)|*.jpg;*.jpeg|" +
              "Portable Network Graphic (.png)|.png";

            if (op.ShowDialog() == true)
            {
                var img = System.Drawing.Image.FromFile(op.FileName);
                var file =
                    DateBox.Text = File.GetCreationTime(op.FileName).ToShortDateString();
                ImgPhoto.Source = new BitmapImage(new Uri(op.FileName));
                FormatBox.Text = System.IO.Path.GetExtension(op.FileName).ToString();
                NameBox.Text = System.IO.Path.GetFileNameWithoutExtension(op.FileName);
                WidthRun.Text = img.Width.ToString();
                HeightRun.Text = img.Height.ToString();



            }


        }

        private void Save_buttonClick(object sender, RoutedEventArgs e)
        {
        }

        private void Tag_ItemClick(object sender, RoutedEventArgs e)
        {
            TagBoxItem.Visibility = System.Windows.Visibility.Visible;
            OkItem.Visibility = System.Windows.Visibility.Visible;
            TagBoxItem.Text = "Predmet_meno";

            TagButtonPerson.Visibility = System.Windows.Visibility.Hidden;
            TagButtonItem.Visibility = System.Windows.Visibility.Hidden;
        }

        private void Tag_PersonClick(object sender, RoutedEventArgs e)
        {
            TagBoxPersonName.Visibility = System.Windows.Visibility.Visible;
            OkPersonFirstName.Visibility = System.Windows.Visibility.Visible;
            TagBoxPersonName.Text = "Meno";
            TagButtonPerson.Visibility = System.Windows.Visibility.Hidden;
            TagButtonItem.Visibility = System.Windows.Visibility.Hidden;

        }
        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            TagBoxItem.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonName.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonSureName.Visibility = System.Windows.Visibility.Hidden;
            // Capture and track the mouse.
            mouseDown = true;
            mouseDownPos = e.GetPosition(theGrid);
            theGrid.CaptureMouse();

            // Initial placement of the drag selection box.         
            Canvas.SetLeft(selectionBox, mouseDownPos.X);
            Canvas.SetTop(selectionBox, mouseDownPos.Y);
            selectionBox.Width = 0;
            selectionBox.Height = 0;

            // Make the drag selection box visible.
            selectionBox.Visibility = Visibility.Visible;
        }

        private void Grid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            // Release the mouse capture and stop tracking it.
            mouseDown = false;
            theGrid.ReleaseMouseCapture();

            // Hide the drag selection box.
            selectionBox.Visibility = Visibility.Collapsed;

            Point mouseUpPos = e.GetPosition(theGrid);
            Canvas.SetTop(TagBoxItem, mouseDownPos.Y + 20);
            Canvas.SetLeft(TagBoxItem, mouseDownPos.X);
            Canvas.SetTop(TagBoxPersonName, mouseDownPos.Y + 20);
            Canvas.SetLeft(TagBoxPersonName, mouseDownPos.X);
            Canvas.SetTop(TagBoxPersonSureName, mouseDownPos.Y + 20);
            Canvas.SetLeft(TagBoxPersonSureName, mouseDownPos.X);
            Canvas.SetTop(CancelButton, mouseDownPos.Y + 60);
            Canvas.SetLeft(CancelButton, mouseDownPos.X);
            Canvas.SetTop(TagButtonItem, mouseDownPos.Y + 40);
            Canvas.SetLeft(TagButtonItem, mouseDownPos.X);
            Canvas.SetTop(TagButtonPerson, mouseDownPos.Y + 20);
            Canvas.SetLeft(TagButtonPerson, mouseDownPos.X);
            Canvas.SetTop(OkItem, mouseDownPos.Y + 40);
            Canvas.SetLeft(OkItem, mouseDownPos.X);
            Canvas.SetTop(OkPersonFirstName, mouseDownPos.Y + 40);
            Canvas.SetLeft(OkPersonFirstName, mouseDownPos.X);
            Canvas.SetTop(OkPerson, mouseDownPos.Y + 40);
            Canvas.SetLeft(OkPerson, mouseDownPos.X);
            TagButtonPerson.Visibility = System.Windows.Visibility.Visible;
            TagButtonItem.Visibility = System.Windows.Visibility.Visible;
            CancelButton.Visibility = System.Windows.Visibility.Visible;

            // TODO: 
            //
            // The mouse has been released, check to see if any of the items 
            // in the other canvas are contained within mouseDownPos and 
            // mouseUpPos, for any that are, select them!
            //
        }

        private void Grid_MouseMove(object sender, MouseEventArgs e)
        {
            if (mouseDown)
            {
                // When the mouse is held down, reposition the drag selection box.

                Point mousePos = e.GetPosition(theGrid);

                if (mouseDownPos.X < mousePos.X)
                {
                    Canvas.SetLeft(selectionBox, mouseDownPos.X);
                    selectionBox.Width = mousePos.X - mouseDownPos.X;
                }
                else
                {
                    Canvas.SetLeft(selectionBox, mousePos.X);
                    selectionBox.Width = mouseDownPos.X - mousePos.X;
                }

                if (mouseDownPos.Y < mousePos.Y)
                {
                    Canvas.SetTop(selectionBox, mouseDownPos.Y);
                    selectionBox.Height = mousePos.Y - mouseDownPos.Y;
                }
                else
                {
                    Canvas.SetTop(selectionBox, mousePos.Y);
                    selectionBox.Height = mouseDownPos.Y - mousePos.Y;
                }
            }
        }
        private void Window_ContentRendered(object sender, EventArgs e)
        {
            TagBoxItem.SelectAll();
            TagBoxItem.Focus();
            TagBoxPersonName.SelectAll();
            TagBoxPersonName.Focus();
            TagBoxPersonSureName.SelectAll();
            TagBoxPersonSureName.Focus();
        }

        private void Tag_OkPersonFirstNameClick(object sender, RoutedEventArgs e)
        {

            TagBoxPersonName.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonSureName.Visibility = System.Windows.Visibility.Visible;
            OkPersonFirstName.Visibility = System.Windows.Visibility.Hidden;
            OkPerson.Visibility = System.Windows.Visibility.Visible;
            TagBoxPersonSureName.Text = "Priezvisko";

        }

        private void Tag_OkItemClick(object sender, RoutedEventArgs e)
        {
            TagBoxItem.Visibility = System.Windows.Visibility.Hidden;
            OkItem.Visibility = System.Windows.Visibility.Hidden;
            CancelButton.Visibility = System.Windows.Visibility.Hidden;
        }

        private void Tag_OkPersonClick(object sender, RoutedEventArgs e)
        {
            TagBoxPersonSureName.Visibility = System.Windows.Visibility.Hidden;

            OkPerson.Visibility = System.Windows.Visibility.Hidden;
            CancelButton.Visibility = System.Windows.Visibility.Hidden;
        }
        private void CancelTag_Click(object sender, RoutedEventArgs e)
        {
            TagBoxItem.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonName.Visibility = System.Windows.Visibility.Hidden;
            TagBoxPersonSureName.Visibility = System.Windows.Visibility.Hidden;
            TagButtonItem.Visibility = System.Windows.Visibility.Hidden;
            TagButtonPerson.Visibility = System.Windows.Visibility.Hidden;
            CancelButton.Visibility = System.Windows.Visibility.Hidden;
            OkItem.Visibility = System.Windows.Visibility.Hidden;
            OkPersonFirstName.Visibility = System.Windows.Visibility.Hidden;
            OkPerson.Visibility = System.Windows.Visibility.Hidden;
        }
    }
}