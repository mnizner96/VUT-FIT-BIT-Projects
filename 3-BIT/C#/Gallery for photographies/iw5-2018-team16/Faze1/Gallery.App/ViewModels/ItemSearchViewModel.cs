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
    public class ItemSearchViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;

        public ICommand ItemSearchCommand { get; set; }
        public string name;
        public ItemSearchViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            ItemSearchCommand = new RelayCommands(SearchItem);

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

        private void SearchItem()
        {
            messenger.Send(new ItemSearchMessage(Name));
        }

    }
}