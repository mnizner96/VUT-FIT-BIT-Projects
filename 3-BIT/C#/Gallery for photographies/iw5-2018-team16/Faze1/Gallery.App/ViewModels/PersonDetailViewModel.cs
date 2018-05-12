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
    public class PersonDetailViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        public ObservableCollection<PhotographyListModel> Photographies { get; set; } = new ObservableCollection<PhotographyListModel>();
        public ICommand SelectPhotographyCommand { get; }
        public ICommand ListCommand { get; set; }
        public ICommand SortByNameCommand { get; set; }
        public ICommand SortByDateCommand { get; set; }
        public ICommand DeletePersonCommand { get; set; }
        public ICommand BackToPersonListCommand { get; set; }
        private PersonDetailModel detail;
        public bool visible;
        private bool load;


        public PersonDetailModel Detail
        {
            get { return detail; }
            set
            {
                if (Equals(value, Detail)) return;

                detail = value;
                OnPropertyChanged();
            }
        }
        public bool Visible
        {
            get { return visible; }
            set
            {
                if (Equals(value, Visible)) return;

                visible = value;
                OnPropertyChanged();
            }
        }
        public PersonDetailViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectPhotographyCommand = new RelayCommands(PhotographySelectionChanged);
            SortByNameCommand = new RelayCommands(PhotographySortedByName);
            SortByDateCommand = new RelayCommands(PhotographySortedByDate);
            DeletePersonCommand = new RelayCommands(DeletePerson);
            BackToPersonListCommand = new RelayCommands(BackToList);
            ListCommand = new RelayCommands(AlbumList);
            this.messenger.Register<UpdatePhotoMessage>(UpdatePhotoFromList);
            this.messenger.Register<ChangeVisibilityMessage>(VisibilityChanged);
            this.messenger.Register<SelectedPersonMessage>(SelectedPerson);
            this.messenger.Register<FilterByDateMessage>(PhotographyFilteredByDate);
            this.messenger.Register<FilterByFormatMessage>(PhotographyFilteredByFormat);
            this.messenger.Register<FilterByResolutionMessage>(PhotographyFilteredByResolution);
            this.messenger.Register<SearchMessage>(PhotographySearch);
            Visible = false;
            load = true;


        }

        public void OnLoad()
        {
            if (load == true)
            {
                Photographies.Clear();
                if (detail != null)
                {
                    var photographies = galleryRepository.GetAllFromPerson(detail.Locations);
                    foreach (var photo in photographies)
                    {
                        Photographies.Add(photo);

                    }
                }
            }



        }
        private void PhotographySelectionChanged(object parameter)
        {
            if (parameter is PhotographyListModel photography)
            {
                messenger.Send(new SelectedPhotoInPersonMessage { Id = photography.Id });
            }
        }
        private void PhotographySortedByName(object parameter)
        {
            Photographies.Clear();
            var photographies = galleryRepository.SortByNameInAlbum(Detail.Id);
            foreach (var photo in photographies)
            {
                Photographies.Add(photo);

            }

        }

        private void PhotographySortedByDate(object parameter)
        {
            Photographies.Clear();
            var photographies = galleryRepository.SortByDateInAlbum(Detail.Id);
            foreach (var photo in photographies)
            {
                Photographies.Add(photo);

            }

        }

        private void UpdatePhotoFromList(UpdatePhotoMessage messenger)
        {
            OnLoad();
        }
        private void VisibilityChanged(ChangeVisibilityMessage messenger)
        {
            Visible = messenger.Changer;
        }

        private void SelectedPerson(SelectedPersonMessage messenger)
        {
            Detail = galleryRepository.GetPersonById(messenger.Id);
            load = true;
        }

        private void PhotographyFilteredByDate(FilterByDateMessage messenger)
        {
            if (Visible == true)
            {
                Photographies.Clear();
                var photographies = galleryRepository.GetAllFromPerson(Detail.Locations);
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Time == messenger.Time)
                        Photographies.Add(photo);

                }
            }

        }
        private void PhotographyFilteredByFormat(FilterByFormatMessage messenger)
        {
            if (Visible == true)
            {
                Photographies.Clear();
                var photographies = galleryRepository.GetAllFromPerson(Detail.Locations);
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Format == messenger.Format)
                        Photographies.Add(photo);
                }
            }
        }
        private void PhotographyFilteredByResolution(FilterByResolutionMessage messenger)
        {
            if (Visible == true)
            {
                Photographies.Clear();
                var photographies = galleryRepository.GetAllFromPerson(Detail.Locations);
                foreach (var photo in photographies)
                {
                    string resolution = galleryRepository.GetById(photo.Id).Height.ToString() + "x" + galleryRepository.GetById(photo.Id).Weight.ToString();
                    if (resolution == messenger.Resolution)
                        Photographies.Add(photo);
                }
            }

        }
        private void PhotographySearch(SearchMessage messenger)
        {
            if (Visible == true)
            {
                Photographies.Clear();
                var photographies = galleryRepository.GetAllFromPerson(Detail.Locations);
                foreach (var photo in photographies)
                {
                    if (galleryRepository.GetById(photo.Id).Name == messenger.Name)
                        Photographies.Add(photo);
                }
            }
            
        }

        private void AlbumList()
        {
            OnLoad();
        }

        private void DeletePerson()
        {
            galleryRepository.DeletePerson(Detail.Id);
            messenger.Send(new UpdatePersonMessage());
            MessageBox.Show("Person" + Detail.LastName + "was sucessfully deleted", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);

            Visible = false;
            load = false;
        }
        private void BackToList()
        {
            messenger.Send(new UpdatePersonMessage());

            Visible = false;
        }

    }
}
