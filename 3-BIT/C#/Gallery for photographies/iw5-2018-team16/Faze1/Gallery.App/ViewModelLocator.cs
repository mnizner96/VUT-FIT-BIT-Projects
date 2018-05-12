using Gallery.App.ViewModels;
using Gallery.BL;
using Gallery.BL.Repository;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.App
{
    public class ViewModelLocator
    {
        private readonly Messenger messenger = new Messenger();
        private readonly GalleryRepository galleryRepository = new GalleryRepository();
        public MainViewModel MainViewModel => CreateMainViewModel();

        public PhotographyDetailViewModel PhotographyDetailViewModel => CreatePhotographyDetailViewModel();

        public PhotographyListViewModel PhotographyListViewModel => CreatePhotographyListViewModel();
        public AlbumDetailViewModel AlbumDetailViewModel => CreateAlbumDetailViewModel();
        public AlbumListViewModel AlbumListViewModel => CreateAlbumListViewModel();
        public AlbumInputViewModel AlbumInputViewModel => CreateAlbumInputViewModel();
        public ChangeNameViewModel ChangeNameViewModel => CreateChangeNameViewModel();
        public PersonDetailViewModel PersonDetailViewModel => CreatePersonDetailViewModel();
        public PersonListViewModel PersonListViewModel => CreatePersonListViewModel();
        public ItemListViewModel ItemListViewModel => CreateItemListViewModel();
        public DateFilterViewModel DateFilterViewModel => CreateDateFilterViewModel();
        public FormatFilterViewModel FormatFilterViewModel => CreateFormatFilterViewModel();
        public ResolutionFilterViewModel ResolutionFilterViewModel => CreateResolutionFilterViewModel();
        public SearchViewModel SearchViewModel => CreateSearchViewModel();
        public PersonSearchViewModel PersonSearchViewModel => CreatePersonSearchViewModel();
        public ItemSearchViewModel ItemSearchViewModel => CreateItemSearchViewModel();
        private MainViewModel CreateMainViewModel()
        {
            return new MainViewModel(messenger);
        }

        private PhotographyDetailViewModel CreatePhotographyDetailViewModel()
        {
            return new PhotographyDetailViewModel(galleryRepository, messenger);
        }

        private PhotographyListViewModel CreatePhotographyListViewModel()
        {
            return new PhotographyListViewModel(galleryRepository, messenger);
        }
        private AlbumDetailViewModel CreateAlbumDetailViewModel()
        {
            return new AlbumDetailViewModel(galleryRepository, messenger);

        }

        private AlbumListViewModel CreateAlbumListViewModel()
        {
            return new AlbumListViewModel(galleryRepository, messenger);
        }

        private ChangeNameViewModel CreateChangeNameViewModel()
        {
            return new ChangeNameViewModel(messenger);
        }
        
        private PersonDetailViewModel CreatePersonDetailViewModel()
        {
            return new PersonDetailViewModel(galleryRepository, messenger);

        }
        
        private PersonListViewModel CreatePersonListViewModel()
        {
            return new PersonListViewModel(galleryRepository, messenger);
        }
        private ItemListViewModel CreateItemListViewModel()
        {
            return new ItemListViewModel(galleryRepository, messenger);
        }
        private AlbumInputViewModel CreateAlbumInputViewModel()
        {
            return new AlbumInputViewModel(galleryRepository, messenger);
        }

        private DateFilterViewModel CreateDateFilterViewModel()
        {
            return new DateFilterViewModel(messenger);
        }
        private FormatFilterViewModel CreateFormatFilterViewModel()
        {
            return new FormatFilterViewModel(messenger);
        }
        private ResolutionFilterViewModel CreateResolutionFilterViewModel()
        {
            return new ResolutionFilterViewModel(messenger);
        }
        private SearchViewModel CreateSearchViewModel()
        {
            return new SearchViewModel(messenger);
        }
        private PersonSearchViewModel CreatePersonSearchViewModel()
        {
            return new PersonSearchViewModel(messenger);
        }
        private ItemSearchViewModel CreateItemSearchViewModel()
        {
            return new ItemSearchViewModel(messenger);
        }
    }
}

