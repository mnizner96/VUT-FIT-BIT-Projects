using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media.Imaging;
using Faze1;
using Gallery.BL.Models;
using Gallery.DAL.Entities;

namespace Gallery.BL
{
    public class Mapper
    {
        [System.Runtime.InteropServices.DllImport("gdi32.dll")]
        public static extern bool DeleteObject(IntPtr hObject);
        public PersonListModel MapEntityToPersonListModel(Person entity)
        {
            return new PersonListModel()
            {
                Id = entity.Id,
                FirstName = entity.FirstName,
                LastName = entity.LastName
            };
        }
        public ItemListModel MapEntityToItemListModel(Item entity)
        {
            return new ItemListModel()
            {
                Id = entity.Id,
                Name = entity.Name
            };
        }
        public AlbumListModel MapEntityToListModel(Album entity)
        {
            return new AlbumListModel()
            {
                Id = entity.Id,
                Name = entity.Name
            };
        }

        public AlbumDetailModel MapEntityToAlbumDetailModel(Album entity)
        {
            return new AlbumDetailModel()
            {
                Id = entity.Id,
                Name = entity.Name
                
            };
        }

        public PhotographyListModel MapEntityToPhotographyListModel(Photography entity)
        {
            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.BeginInit();
            bitmapImage.StreamSource = new System.IO.MemoryStream(entity.Image);
            bitmapImage.EndInit();







            return new PhotographyListModel()
            {
                Id = entity.PhotographyId,
                Image = bitmapImage
            };
        }

        public PhotographyDetailModel MapEntityToPhotographyDetailModel(Photography entity)
        {
            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.BeginInit();
            bitmapImage.StreamSource = new System.IO.MemoryStream(entity.Image);
            bitmapImage.EndInit();

            return new PhotographyDetailModel()
            {
                Id = entity.PhotographyId,
                Image=bitmapImage,
                Name = entity.Name,
                Time = entity.Time,
                Format = entity.Format,
                Weight = entity.Weight,
                Height = entity.Height,
                Additional = entity.Additional,
                Locations = entity.Locations,
                Album = entity.Album

            };
        }

        public Photography MapPhotogrpahyDetailModelToEntity(PhotographyDetailModel entity)
        {
            byte[] data;
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(entity.Image));
            using (MemoryStream ms = new MemoryStream())
            {
                encoder.Save(ms);
                data = ms.ToArray();
            }
            return new Photography()
            { 
                PhotographyId = entity.Id,
                Image=data,
                Name = entity.Name,
                Time = entity.Time,
                Format = entity.Format,
                Weight = entity.Weight,
                Height = entity.Height,
                Additional = entity.Additional,
                Locations = entity.Locations,
                Album = entity.Album
            };
        }

        public Album MapAlbumDetailModelToEntity(AlbumDetailModel entity)
        {
            return new Album()
            {
                Id = entity.Id,
                
            };
        } 

        public Person MapPersonDetailModelToEntity(PersonDetailModel entity)
        {
            return new Person()
            {
                Id = entity.Id,
                FirstName = entity.FirstName,
                LastName = entity.LastName
            };
        }

        public Item MapItemDetailModelToEntity(ItemDetailModel entity)
        {
            return new Item()
            {
                Id = entity.Id,
                Name = entity.Name
                
            };
        }
        public Location MapLocationDetailToEntity(LocationDetailModel entity)
        {
            return new Location()
            {
                Id = entity.Id,
                Width = entity.Width,
                Height = entity.Height

            };

        }

        public PersonDetailModel MapEntityToPersonDetailModel(Person entity)
        {
            return new PersonDetailModel()
            {
                Id = entity.Id,
                FirstName = entity.FirstName,
                LastName = entity.LastName,
                Locations = entity.Locations
            };
        }
    }
}
