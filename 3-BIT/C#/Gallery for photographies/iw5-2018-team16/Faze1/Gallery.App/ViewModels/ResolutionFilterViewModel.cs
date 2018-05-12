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
    public class ResolutionFilterViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;

        public ICommand FilterByResolutionCommand { get; set; }
        public string heighttofilter;
        public string widthtofilter;
        public ResolutionFilterViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            FilterByResolutionCommand = new RelayCommands(FilterByResolution);

        }
        public string HeightToFilter
        {
            get { return heighttofilter; }
            set
            {
                if (Equals(value, HeightToFilter)) return;

                heighttofilter = value;
                OnPropertyChanged();
            }
        }

        public string WidthToFilter
        {
            get { return widthtofilter; }
            set
            {
                if (Equals(value, WidthToFilter)) return;

                widthtofilter = value;
                OnPropertyChanged();
            }
        }

        public void OnLoad()
        {

        }

        private void FilterByResolution()
        {
            string resolution = HeightToFilter + "x" + WidthToFilter;
            messenger.Send(new FilterByResolutionMessage(resolution));
        }

    }
}