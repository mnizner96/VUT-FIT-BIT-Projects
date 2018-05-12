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
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class PhotographyListViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        public ObservableCollection<PhotographyListModel> Photographies { get; set; } = new ObservableCollection<PhotographyListModel>();
        public ICommand SelectPhotographyCommand { get; }
        public ICommand SortByNameCommand { get; set; }
        public ICommand SortByDateCommand { get; set; }
        public ICommand ListCommand { get; set; }
        
        
       
        public PhotographyListViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectPhotographyCommand = new RelayCommands(PhotographySelectionChanged);
            SortByNameCommand = new RelayCommands(PhotographySortedByName);
            SortByDateCommand = new RelayCommands(PhotographySortedByDate);
            ListCommand = new RelayCommands(PhotographyList);
            this.messenger.Register<UpdatePhotoMessage>(UpdatePhotoFromList);
            this.messenger.Register<FilterByDateMessage>(PhotographyFilteredByDate);
            this.messenger.Register<FilterByFormatMessage>(PhotographyFilteredByFormat);
            this.messenger.Register<FilterByResolutionMessage>(PhotographyFilteredByResolution);
            this.messenger.Register<SearchMessage>(PhotographySearch);
          
           

        }

        public void OnLoad()
        {
            Photographies.Clear();
            var photographies = galleryRepository.GetAll();
            foreach (var photo in photographies)
            {
                Photographies.Add(photo);

            }
        }
        private void PhotographyList()
        {
            OnLoad();
        }
        private void PhotographySelectionChanged(object parameter)
        {
            if (parameter is PhotographyListModel photography)
            {
                messenger.Send(new SelectedPhotoInMainMessage { Id = photography.Id });

            }
        }
        private void PhotographySortedByName(object parameter)
        {
            Photographies.Clear();
            var photographies = galleryRepository.SortByName();
            foreach (var photo in photographies)
            {
                Photographies.Add(photo);

            }

        }

        private void PhotographySortedByDate(object parameter)
        {
            Photographies.Clear();
            var photographies = galleryRepository.SortByDate();
            foreach (var photo in photographies)
            {
                Photographies.Add(photo);

            }

        }
       
        private void UpdatePhotoFromList(UpdatePhotoMessage messenger)
        {
            OnLoad();
        }

        private void PhotographyFilteredByDate(FilterByDateMessage messenger)
        {
            
                Photographies.Clear();
                var photographies = galleryRepository.GetAll();
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Time == messenger.Time)
                        Photographies.Add(photo);

                }
            

        }
        private void PhotographyFilteredByFormat(FilterByFormatMessage messenger)
        {
                Photographies.Clear();
                var photographies = galleryRepository.GetAll();
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Format == messenger.Format)
                        Photographies.Add(photo);
                }
            
        }
        private void PhotographyFilteredByResolution(FilterByResolutionMessage messenger)
        {
            
                Photographies.Clear();
                var photographies = galleryRepository.GetAll();
                foreach (var photo in photographies)
                {
                    string resolution = galleryRepository.GetById(photo.Id).Height.ToString() + "x" + galleryRepository.GetById(photo.Id).Weight.ToString();
                    if (resolution == messenger.Resolution)
                        Photographies.Add(photo);
                }
            
        }
        private void PhotographySearch(SearchMessage messenger)
        {
           
                Photographies.Clear();
                var photographies = galleryRepository.GetAll();
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Name == messenger.Name)
                        Photographies.Add(photo);
                }
            
        }

    }
}
