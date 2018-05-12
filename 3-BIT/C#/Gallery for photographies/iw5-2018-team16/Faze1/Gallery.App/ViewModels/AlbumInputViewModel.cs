using Gallery.App.Commands;
using Gallery.BL;
using Gallery.BL.Messages;
using Gallery.BL.Repository;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class AlbumInputViewModel : ViewModelBase
    {
        private readonly IMessenger messenger;
        private readonly GalleryRepository galleryRepository;

        public ICommand AddNewAlbumCommand { get; set; }
        public string name;
        public AlbumInputViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            AddNewAlbumCommand = new RelayCommands(AddedNewAlbum);

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

        private void AddedNewAlbum()
        {
            galleryRepository.CreateAlbum(Name);
            messenger.Send(new UpdateAlbumMessage());
        }

    }
}


