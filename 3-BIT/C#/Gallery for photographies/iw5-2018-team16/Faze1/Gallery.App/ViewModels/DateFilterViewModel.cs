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
    public class DateFilterViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;
      
        public ICommand FilterByDateCommand { get; set; }
        public string timetofilter;
        public DateFilterViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            FilterByDateCommand = new RelayCommands(FilterByDate);

        }
        public string TimeToFilter
        {
            get { return timetofilter; }
            set
            {
                if (Equals(value, TimeToFilter)) return;

                timetofilter = value;
                OnPropertyChanged();
            }
        }

        public void OnLoad()
        {

        }

        private void FilterByDate()
        {
            messenger.Send(new FilterByDateMessage(TimeToFilter));
        }

    }
}
