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
    public class SearchViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;

        public ICommand SearchCommand { get; set; }
        public string name;
        public SearchViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            SearchCommand = new RelayCommands(SearchImage);

        }
        public string Name
        {
            get { return name; }
            set
            {
                if (Equals(value, Name)) return;

                name = value;
                OnPropertyChanged();
            }
        }

        public void OnLoad()
        {

        }

        private void SearchImage()
        {
            messenger.Send(new SearchMessage(Name));
        }

    }
}
