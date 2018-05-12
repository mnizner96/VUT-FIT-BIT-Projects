using Gallery.App.Commands;
using Gallery.BL;
using Gallery.BL.Messages;
using Gallery.BL.Models;
using Gallery.BL.Repository;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class AlbumListViewModel: ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        private PhotographyDetailModel model;
        public ObservableCollection<AlbumListModel> Albums { get; set; } = new ObservableCollection<AlbumListModel>();
        public ICommand SelectAlbumCommand { get; }
        
        private bool selectionforchoosealbum;


        public AlbumListViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            selectionforchoosealbum = false;
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectAlbumCommand = new RelayCommands(AlbumSelectionChanged);
            this.messenger.Register<UpdateAlbumMessage>(UpdateAlbumFromList);
            this.messenger.Register<AddToAlbumMessage>(ChooseAlbumForPhoto);
        }

        public void OnLoad()
        {
            if (selectionforchoosealbum == false )
            {
                messenger.Send(new HideDetailMessage());
            }
            Albums.Clear();
            var albums = galleryRepository.GetAllAlbum();
            foreach (var album in albums)
            {
                Albums.Add(album);
            }
        }
        private void AlbumSelectionChanged(object parameter)
        {
            if (parameter is AlbumListModel album)
            {
                if (selectionforchoosealbum == false)
                { 
                    messenger.Send(new SelectedAlbumMessage { Id = album.Id });
                    messenger.Send(new UpdatePhotoMessage(null));
                    messenger.Send(new ChangeVisibilityMessage(true));

                }
                else if (selectionforchoosealbum == true)
                {
                    selectionforchoosealbum = false;
                    try
                    {
                        if (!galleryRepository.GetById(model.Id).Album.Equals(null))   // I've also used if(data != null) which hasn't worked either
                        {
                            MessageBox.Show("This photo already contains another album(" + model.Album.Name + ")", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);

                        }
                    }
                    catch (Exception)
                    {
                        galleryRepository.AddPhotoToAlbum(album.Id, model.Id);
                        this.messenger.Send(new UpdatePhotoMessage(model));
                        MessageBox.Show("Photo was sucessfully saved to Album", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
                        messenger.Send(new HideDetailMessage());
                    }
                        this.messenger.Send(new ChangeTabItemMessage(0));
                    



                }

            }
        }
        private void UpdateAlbumFromList(UpdateAlbumMessage messenger)
        {
            OnLoad();
        }
        private void ChooseAlbumForPhoto(AddToAlbumMessage messenger)
        {
            selectionforchoosealbum = true;
            model = messenger.Model;
        }

        
    }
}
