using Gallery.BL;
using Gallery.BL.Repository;
using Gallery.BL.Messages;
using Gallery.App;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Gallery.App.ViewModels;
using System.Windows;

namespace Gallery.App.Commands
{
    public class SavePhotoCommand : ICommand
    {
        private readonly GalleryRepository galleryRepository;
        private readonly PhotographyDetailViewModel viewModel;
        private readonly IMessenger messenger;
        public SavePhotoCommand(GalleryRepository galleryRepository, PhotographyDetailViewModel viewModel, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.viewModel = viewModel;
            this.messenger = messenger;
        }

        public bool CanExecute(object parameter)
        {
            
            return true;
        }

        public void Execute(object parameter)
        {
            if (viewModel.Detail.Id == Guid.Empty)
            {
                if (galleryRepository.GetByName(viewModel.Detail.Name) != null)
                {
                    MessageBox.Show("Error with save(Database contains photo with same name", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);

                }
                else if (viewModel.Detail.Name == "" || viewModel.Detail.Image == null)
                {
                    MessageBox.Show("Error with save(Some attributes (name or image) are missing", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    galleryRepository.InsertPhoto(viewModel.Detail);
                    MessageBox.Show("Photo was sucessfully saved", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
                    messenger.Send(new UpdatePhotoMessage(viewModel.Detail));
                    messenger.Send(new HideDetailMessage());


                }
            }
            else
            {
                if (viewModel.Detail.Name == "" || viewModel.Detail.Image == null)
                {
                    MessageBox.Show("Error with save(Some attributes (name or image) are missing", "Alert", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    if (viewModel.AlbumOrPhoto == false)
                    {
                        galleryRepository.AddAdditionalInfo(viewModel.Detail);
                    }
                    else if (viewModel.AlbumOrPhoto == true)
                    {
                        galleryRepository.AddAdditionalInfoToAlbum(viewModel.Detail, viewModel.Detail.Album.Id);
                    }
                    MessageBox.Show("Photo was sucessfully updated", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);
                    messenger.Send(new UpdatePhotoMessage(viewModel.Detail));
                    messenger.Send(new HideDetailMessage());


                }

                
            }

        }

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }
    }

   
}