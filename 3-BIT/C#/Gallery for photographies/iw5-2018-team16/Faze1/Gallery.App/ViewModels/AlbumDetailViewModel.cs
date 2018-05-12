
using Gallery.App.Commands;
using Gallery.BL;
using Gallery.BL.Messages;
using Gallery.BL.Models;
using Gallery.BL.Repository;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace Gallery.App.ViewModels
{
    public class AlbumDetailViewModel : ViewModelBase
    {
        private readonly GalleryRepository galleryRepository;
        private readonly IMessenger messenger;
        public ObservableCollection<PhotographyListModel> Photographies { get; set; } = new ObservableCollection<PhotographyListModel>();
        public ICommand SelectPhotographyCommand { get; }
        public ICommand ListCommand { get; set; }
        public ICommand SortByNameCommand { get; set; }
        public ICommand SortByDateCommand { get; set; }
        public ICommand DeleteAlbumCommand { get; set; }
        public ICommand ChangeNameCommand { get; set; }
        public ICommand BackToAlbumListCommand { get; set; }
        private AlbumDetailModel detail;
        private bool visible;
        private bool load;


        public AlbumDetailModel Detail
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
        public AlbumDetailViewModel(GalleryRepository galleryRepository, IMessenger messenger)
        {
            this.galleryRepository = galleryRepository;
            this.messenger = messenger;
            SelectPhotographyCommand = new RelayCommands(PhotographySelectionChanged);
            SortByNameCommand = new RelayCommands(PhotographySortedByName);
            SortByDateCommand = new RelayCommands(PhotographySortedByDate);
            DeleteAlbumCommand = new RelayCommands(DeleteAlbum);
            BackToAlbumListCommand = new RelayCommands(BackToList);
        
            ListCommand = new RelayCommands(AlbumList);
            this.messenger.Register<UpdatePhotoMessage>(UpdatePhotoFromList);
            this.messenger.Register<ChangeVisibilityMessage>(VisibilityChanged);
            this.messenger.Register<SelectedAlbumMessage>(SelectedAlbum);
            this.messenger.Register<FilterByDateMessage>(PhotographyFilteredByDate);
            this.messenger.Register<FilterByFormatMessage>(PhotographyFilteredByFormat);
            this.messenger.Register<FilterByResolutionMessage>(PhotographyFilteredByResolution);
            this.messenger.Register<SearchMessage>(PhotographySearch);
            this.messenger.Register<ChangeNameMessage>(ChangeName);
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
                    var photographies = galleryRepository.GetAllFromAlbum(detail.Id);
                    foreach (var photo in photographies)
                    {
                        Photographies.Add(photo);

                    }
                }
            }
        }
        private void ChangeName(ChangeNameMessage messenger)
        {
            galleryRepository.UpdateAlbum(messenger.Name, Detail.Id);
            
        }
        private void PhotographySelectionChanged(object parameter)
        {
            if (parameter is PhotographyListModel photography)
            {
                messenger.Send(new SelectedPhotoInAlbumMessage { Id = photography.Id });
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

        private void SelectedAlbum(SelectedAlbumMessage messenger)
        {
            Detail = galleryRepository.GetAlbumById(messenger.Id);
            load = true;
        }

        private void PhotographyFilteredByDate(FilterByDateMessage messenger)
        {
            if (Visible == true)
            {
                Photographies.Clear();
                var photographies = galleryRepository.GetAllFromAlbum(Detail.Id);
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
                var photographies = galleryRepository.GetAllFromAlbum(Detail.Id);
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
                var photographies = galleryRepository.GetAllFromAlbum(Detail.Id);
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
                var photographies = galleryRepository.GetAllFromAlbum(Detail.Id);
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

        private void DeleteAlbum()
        {
            galleryRepository.DeleteAlbum(Detail.Id);
            messenger.Send(new UpdateAlbumMessage());
            MessageBox.Show("Album" + Detail.Name + "was sucessfully deleted", "Alert", MessageBoxButton.OK, MessageBoxImage.Information);

            Visible = false;
            load = false;
        }
        private void BackToList()
        {
            messenger.Send(new UpdateAlbumMessage());

            Visible = false;
        }
        

    }
}
