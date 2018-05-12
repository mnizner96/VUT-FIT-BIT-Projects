using Gallery.BL;
using Gallery.App.Commands;
using Gallery.BL.Messages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;
        public string Name { get; set; } = "Nenacteno";
        public ICommand AddPhotoCommand { get; set; }
        public ICommand AddAlbumCommand { get; set; }
        public ICommand SwapToPhotoCommand { get; set; }
        
        

        public int tabindex;

        public int TabIndex
        {
            get { return tabindex; }
            set
            {
                if (Equals(value, TabIndex)) return;
                tabindex = value;
                OnPropertyChanged();
            }
        }
        public MainViewModel (IMessenger messenger)
        {
            this.messenger = messenger;
            AddPhotoCommand = new RelayCommands(AddNewPhoto);
            SwapToPhotoCommand = new RelayCommands(SwapPhoto);
        

            this.messenger.Register<ChangeTabItemMessage>(ChangeTabItem);
        }

        private void AddNewPhoto()
        {
            messenger.Send(new AddPhotoMessage());
        }

       

       

        private void SwapPhoto()
        {
            messenger.Send(new ChangeVisibilityMessage(false));
            
        }

        
       
        private void ChangeTabItem(ChangeTabItemMessage messenger)
        {
            TabIndex = messenger.Changer;
        }
        public void OnLoad()
        {

        }

    }
}
