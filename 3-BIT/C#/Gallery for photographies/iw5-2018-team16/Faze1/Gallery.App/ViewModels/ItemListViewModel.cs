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
    public class ItemListViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        private PhotographyDetailModel model;
        public ObservableCollection<ItemListModel> Items { get; set; } = new ObservableCollection<ItemListModel>();
        public ICommand SelectItemCommand { get; }
        public ICommand ListItemCommand { get; set; }

        private bool selection_for_chosen_item;


        public ItemListViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            selection_for_chosen_item = false;
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectItemCommand = new RelayCommands(ItemSelectionChanged);
            ListItemCommand = new RelayCommands(ItemList);
            this.messenger.Register<ItemSearchMessage>(ItemSearch);

        }

        public void OnLoad()
        {
            if (selection_for_chosen_item == false)
            {
                messenger.Send(new HideDetailMessage());
            }
            Items.Clear();
            var items = galleryRepository.GetAllItem();
            foreach (var item in items)
            {
                Items.Add(item);
            }
        }
        private void ItemSelectionChanged(object parameter)
        {
            if (parameter is ItemListModel item)
            {
                if (selection_for_chosen_item == false)
                {
                    messenger.Send(new SelectedItemMessage { Id = item.Id });
                    messenger.Send(new UpdatePhotoMessage(null));
                    messenger.Send(new ChangeVisibilityMessage(true));

                }
                else if (selection_for_chosen_item == true)
                {
                    selection_for_chosen_item = false;
                    this.messenger.Send(new ChangeTabItemMessage(0));




                }

            }
        }
        private void ItemList()
        {
            OnLoad();
        }
        /*
        private void UpdateAlbumFromList(UpdateAlbumMessage messenger)
        {
            OnLoad();
        }
        */
        private void ItemSearch(ItemSearchMessage messenger)
        {
            Items.Clear();
            var items = galleryRepository.GetAllItem();
            foreach (var item in items)
            {
                if ((galleryRepository.GetItemById(item.Id).Name == messenger.Name))
                    Items.Add(item);
            }
        }

    }

}
