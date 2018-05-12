using Gallery.App.Commands;
using Gallery.BL;
using Gallery.BL.Messages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class FormatFilterViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;
        public ICommand FilterByFormatCommand { get; set; }
        public bool jpgfilter;
        public bool jepgfilter;
        public bool pngfilter;
        public FormatFilterViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            jpgfilter = false;
            jepgfilter = false;
            pngfilter = false;
            FilterByFormatCommand = new RelayCommands(FilterByFormat);
        }
        public bool JpgFilter
        {
            get { return jpgfilter; }
            set
            {
                if (Equals(value, JpgFilter)) return;

                jpgfilter = value;
                OnPropertyChanged();
            }
        }
        public bool JepgFilter
        {
            get { return jepgfilter; }
            set
            {
                if (Equals(value, JepgFilter)) return;

                jepgfilter = value;
                OnPropertyChanged();
            }
        }
        public bool PngFilter
        {
            get { return pngfilter; }
            set
            {
                if (Equals(value, PngFilter)) return;

                pngfilter = value;
                OnPropertyChanged();
            }
        }
        private void FilterByFormat()
        {
            if (JpgFilter == true)
                messenger.Send(new FilterByFormatMessage(".jpg"));
            if (JepgFilter == true)
                messenger.Send(new FilterByFormatMessage(".jepg"));
            if (PngFilter == true)
                messenger.Send(new FilterByFormatMessage(".png"));
        }
    }
}