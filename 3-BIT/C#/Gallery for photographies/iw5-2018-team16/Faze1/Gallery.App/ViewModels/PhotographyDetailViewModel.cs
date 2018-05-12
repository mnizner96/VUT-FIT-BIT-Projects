using Gallery.BL;
using Gallery.App.Commands;
using Gallery.BL.Messages;
using Gallery.BL.Repository;
using Gallery.App.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Gallery.BL.Models;
using System.Windows;

namespace Gallery.App.ViewModels
{
    public class PhotographyDetailViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        private PhotographyDetailModel detail;
        public bool AlbumOrPhoto;
        private ItemDetailModel itemdetail;
        private PersonDetailModel persondetail;
        private LocationDetailModel locationdetail;
        public PhotographyDetailModel Detail
        {
            get { return detail; }
            set
            {
                if (Equals(value, Detail)) return;

                detail = value;
                OnPropertyChanged();
            }
        }

        public ItemDetailModel ItemDetail
        {
            get { return itemdetail; }
            set
            {
                if (Equals(value, ItemDetail)) return;

                itemdetail = value;
                OnPropertyChanged();
            }
        }

        public PersonDetailModel PersonDetail
        {
            get { return persondetail; }
            set
            {
                if (Equals(value, PersonDetail)) return;

                persondetail = value;
                OnPropertyChanged();
            }
        }
        public LocationDetailModel LocationDetail
        {
            get { return locationdetail; }
            set
            {
                if (Equals(value, LocationDetail)) return;

                locationdetail = value;
                OnPropertyChanged();
            }
        }


        public ICommand SavePhotoCommand { get; set; }
        public ICommand DeletePhotoCommand { get; set; }
        public ICommand AddToAlbumCommand { get; set; }
        public ICommand SaveLocationItemCommand { get; set; }

        public ICommand SaveLocationPersonCommand { get; set; }
        public PhotographyDetailViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;

            SavePhotoCommand = new SavePhotoCommand(galleryRepository, this, messenger);
            DeletePhotoCommand = new RelayCommands(DeletedPhoto);
            AddToAlbumCommand = new RelayCommands(AddedToAlbum);
            SaveLocationItemCommand = new RelayCommands(SaveLocationItem);
            SaveLocationPersonCommand = new RelayCommands(SaveLocationPerson);

            this.messenger.Register<SelectedPhotoInMainMessage>(SelectedPhotoInMain);
            this.messenger.Register<SelectedPhotoInAlbumMessage>(SelectedPhotoInAlbum);
            this.messenger.Register<AddPhotoMessage>(NewPhotoMessageReceived);
            this.messenger.Register<HideDetailMessage>(HideDetailMessageReceived);
         
            
        }
        private void DeletedPhoto()
        {
            if (Detail.Id != Guid.Empty)
            {
                var photoId = Detail.Id;

                if (AlbumOrPhoto == false)
                {
                    galleryRepository.DeletePhoto(photoId);
                }
                else if (AlbumOrPhoto == true)
                {
                    var photoAlbum = Detail.Album.Id;

                    galleryRepository.DeletePhotoFromAlbum(photoId, photoAlbum);
                }
                Detail = new PhotographyDetailModel();

                messenger.Send(new UpdatePhotoMessage(Detail));
                messenger.Send(new HideDetailMessage());

                MessageBox.Show("Photo was sucessfully deleted", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

      

        private void NewPhotoMessageReceived(AddPhotoMessage obj)
        {
            Detail = new PhotographyDetailModel();
          
        }
        private void SelectedPhotoInMain(SelectedPhotoInMainMessage message)
        {
            Detail = galleryRepository.GetById(message.Id);
            ItemDetail = new ItemDetailModel();
            PersonDetail = new PersonDetailModel();
            LocationDetail = new LocationDetailModel();
            AlbumOrPhoto = false;
        }

        private void SelectedPhotoInAlbum(SelectedPhotoInAlbumMessage message)
        {
            Detail = galleryRepository.GetById(message.Id);
            ItemDetail = new ItemDetailModel();
            PersonDetail = new PersonDetailModel();
            LocationDetail = new LocationDetailModel();


            AlbumOrPhoto = true;
        }
        private void HideDetailMessageReceived(HideDetailMessage message)
        {
            Detail = null;
        }

        private void AddedToAlbum()
        {
            this.messenger.Send(new AddToAlbumMessage(this.Detail));
            this.messenger.Send(new ChangeTabItemMessage(1));
        }

        private void SaveLocationItem()
        {
            if (galleryRepository.AddItemLocation(ItemDetail, LocationDetail, Detail.Id) == false)
            {
                MessageBox.Show("Item " + ItemDetail.Name + " is already on this photo", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                MessageBox.Show("Item " + ItemDetail.Name + " was sucessfully marked", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
            }

        }

        private void SaveLocationPerson()
        {
            if (galleryRepository.AddPersonLocation(PersonDetail, LocationDetail, Detail.Id) == false)
            {
                MessageBox.Show("Person " + PersonDetail.FirstName + " is already on this photo", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                MessageBox.Show("Person " + PersonDetail.FirstName + " was sucessfully marked", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
            }
           
        }

    }
}