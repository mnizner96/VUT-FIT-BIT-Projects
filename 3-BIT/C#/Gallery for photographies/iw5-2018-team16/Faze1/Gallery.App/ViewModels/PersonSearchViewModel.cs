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
    public class PersonSearchViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;

        public ICommand PersonSearchCommand { get; set; }
        public string name;
        public PersonSearchViewModel(IMessenger messenger)
        {
            this.messenger = messenger;
            PersonSearchCommand = new RelayCommands(SearchPerson);

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

        private void SearchPerson()
        {
            messenger.Send(new PersonSearchMessage(Name));
        }

    }
}