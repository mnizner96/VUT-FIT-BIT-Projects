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
    public class ChangeNameViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;

        public ICommand ChangeNameCommand { get; set; }
        public string name;
        public ChangeNameViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            ChangeNameCommand = new RelayCommands(ChangeName);

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

        private void ChangeName()
        {
            messenger.Send(new ChangeNameMessage(Name));
        }

    }
}