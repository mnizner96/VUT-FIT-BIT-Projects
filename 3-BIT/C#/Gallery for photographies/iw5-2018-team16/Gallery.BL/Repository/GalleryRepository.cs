using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Faze1;
using System.Data.Entity;
using Gallery.DAL;
using Gallery.BL;
using Gallery.BL.Models;
using Gallery.DAL.Entities;

namespace Gallery.BL.Repository
{
    public class GalleryRepository
    {
         private Mapper mapper = new Mapper(); 
         public void InsertPhoto (PhotographyDetailModel detail)
         {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = mapper.MapPhotogrpahyDetailModelToEntity(detail);
         
                entity.PhotographyId = Guid.NewGuid();
                galleryDbContext.Photographies.Add(entity);
                galleryDbContext.SaveChanges();
                
            }
         }

        public void DeletePhoto(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = new Photography() { PhotographyId = Id };
                galleryDbContext.Photographies.Attach(entity);
                galleryDbContext.Photographies.Remove(entity);
                galleryDbContext.SaveChanges();

            }
        }

        public PhotographyDetailModel GetById(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var PhotographyEntity = galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == Id);
                return mapper.MapEntityToPhotographyDetailModel(PhotographyEntity);
            }
        }

        public AlbumDetailModel GetAlbumById(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {

                var AlbumEntity = galleryDbContext.Albums.FirstOrDefault(r => r.Id == Id);
                return mapper.MapEntityToAlbumDetailModel(AlbumEntity);
            }
        }

        public PhotographyDetailModel GetByName(string Name)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var PhotographyEntity = galleryDbContext.Photographies.FirstOrDefault(r => r.Name == Name);
                return PhotographyEntity==null ? null: mapper.MapEntityToPhotographyDetailModel(PhotographyEntity);
            }
        }
        public PersonDetailModel GetPersonById(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var PersonEntity = galleryDbContext.Persons.FirstOrDefault(r => r.Id == Id);
                return PersonEntity == null ? null : mapper.MapEntityToPersonDetailModel(PersonEntity);
            }
        }
        public ItemListModel GetItemById(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var ItemEntity = galleryDbContext.Items.FirstOrDefault(r => r.Id == Id);
                return ItemEntity == null ? null : mapper.MapEntityToItemListModel(ItemEntity);
            }
        }

        public List<PhotographyListModel> GetAll()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Photographies.Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public List<PhotographyListModel> GetAllFromAlbum(Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Albums.FirstOrDefault(r => r.Id == Id).Photographies.Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public List<PhotographyListModel> SortByDate()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Photographies.OrderByDescending(x=> x.Time).Select(mapper.MapEntityToPhotographyListModel).ToList();
            }       
        }

        public List<PhotographyListModel> SortByDateInAlbum(Guid id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Albums.FirstOrDefault(r=> r.Id == id).Photographies.OrderByDescending(x => x.Time).Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public List<PhotographyListModel> SortByName()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Photographies.OrderBy(x => x.Name).Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public List<PhotographyListModel> SortByNameInAlbum(Guid id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Albums.FirstOrDefault(r=> r.Id==id).Photographies.OrderBy(x => x.Name).Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public void AddAdditionalInfo(PhotographyDetailModel detail)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = galleryDbContext.Photographies.First(r => r.PhotographyId == detail.Id);
                entity.Format = detail.Format;
                entity.Height = detail.Height;
                entity.Name = detail.Name;
                entity.Time = detail.Time;
                entity.Weight = detail.Weight;
                entity.Additional = detail.Additional;
                galleryDbContext.SaveChanges();
            }

        }
        public List<PersonListModel> GetAllPerson()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Persons.Select(mapper.MapEntityToPersonListModel).ToList();
            }
        }

        public List<ItemListModel> GetAllItem()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Items.Select(mapper.MapEntityToItemListModel).ToList();
            }
        }

        public void AddAdditionalInfoToAlbum(PhotographyDetailModel detail, Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = galleryDbContext.Albums.First(r=>r.Id == Id).Photographies.First(r => r.PhotographyId == detail.Id);
                entity.Format = detail.Format;
                entity.Height = detail.Height;
                entity.Name = detail.Name;
                entity.Time = detail.Time;
                entity.Weight = detail.Weight;
                entity.Additional = detail.Additional;
                galleryDbContext.SaveChanges();
            }

        }
        public void CreateAlbum(string name)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = new Album();
                entity.Id = Guid.NewGuid();
                entity.Name = name; 
                galleryDbContext.Albums.Add(entity);
                galleryDbContext.SaveChanges();
            }
        }
        public void UpdateAlbum(string Name, Guid Id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity = galleryDbContext.Albums.First(r => r.Id == Id);
                entity.Name = Name;
                galleryDbContext.SaveChanges();
            }
        }

        public void DeleteAlbum(Guid id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                
                var photographies = galleryDbContext.Albums.FirstOrDefault(r => r.Id == id).Photographies.ToList();
                foreach (var photo in photographies)
                {
                    galleryDbContext.Albums.FirstOrDefault(r => r.Id == id).Photographies.Remove(photo);
                }
                var album = galleryDbContext.Albums.FirstOrDefault(r => r.Id == id);
                galleryDbContext.Albums.Remove(album);
                galleryDbContext.SaveChanges();
            }
        }
        public List<AlbumListModel> GetAllAlbum()
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                return galleryDbContext.Albums.Select(mapper.MapEntityToListModel).ToList();
            }
        }

        public void DeletePhotoFromAlbum(Guid Id_photo, Guid Id_album)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity_photo = new Photography() { PhotographyId = Id_photo };
                galleryDbContext.Photographies.Attach(entity_photo);
                galleryDbContext.Albums.FirstOrDefault(r => r.Id == Id_album).Photographies.Remove(entity_photo);
                galleryDbContext.SaveChanges();

            }
        }

        public void AddPhotoToAlbum(Guid Id_album, Guid Id_photo)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity_photo = new Photography() { PhotographyId = Id_photo };
                galleryDbContext.Photographies.Attach(entity_photo);
                galleryDbContext.Albums.FirstOrDefault(r => r.Id == Id_album).Photographies.Add(entity_photo);
                galleryDbContext.SaveChanges();

            }
        }
        public bool AddPersonLocation(PersonDetailModel person, LocationDetailModel location, Guid photographyid)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity_location = mapper.MapLocationDetailToEntity(location);
                var entity = galleryDbContext.Persons.FirstOrDefault(r => r.FirstName == person.FirstName && r.LastName == person.LastName);
                if (entity == null)
                {
                    entity = mapper.MapPersonDetailModelToEntity(person);
                    entity.Id = Guid.NewGuid();
                    galleryDbContext.Persons.Add(entity);
                    galleryDbContext.SaveChanges();
                    entity_location.Id = Guid.NewGuid();
                    galleryDbContext.Locations.Add(entity_location);

                    galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == photographyid).Locations.Add(entity_location);
                    galleryDbContext.Persons.FirstOrDefault(r => r.Id == entity.Id).Locations.Add(entity_location);
                    galleryDbContext.SaveChanges();
                    return true;

                }
                else
                {
                    bool alreadyContain = false;
                    var photo = galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == photographyid);
                    if (photo.Locations != null)
                    {
                        foreach (var loc in entity.Locations)
                        {
                            if (photo.Locations.FirstOrDefault(r => r.Id == loc.Id) != null)
                            {
                                alreadyContain = true;
                            }
                        }
                    }
                    if (alreadyContain == true)
                    {
                        return false;
                    }
                    else {
                        galleryDbContext.Locations.Add(entity_location);
                        galleryDbContext.Persons.FirstOrDefault(r => r.Id == entity.Id).Locations.Add(entity_location);
                        galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == photographyid).Locations.Add(entity_location);

                        galleryDbContext.SaveChanges();
                        return true;
                    }
                    
                    
                }
                

            }
        }

        public bool AddItemLocation(ItemDetailModel item, LocationDetailModel location, Guid photographyid)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {
                var entity_location = mapper.MapLocationDetailToEntity(location);
                var entity = galleryDbContext.Items.FirstOrDefault(r => r.Name == item.Name);
                if (entity == null)
                {
                    entity = mapper.MapItemDetailModelToEntity(item);
                    entity.Id = Guid.NewGuid();
                    galleryDbContext.Items.Add(entity);
                    galleryDbContext.SaveChanges();
                    entity_location.Id = Guid.NewGuid();
                    galleryDbContext.Locations.Add(entity_location);

                    galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == photographyid).Locations.Add(entity_location);
                    galleryDbContext.Items.FirstOrDefault(r => r.Id == entity.Id).Locations.Add(entity_location);
                    galleryDbContext.SaveChanges();
                    return true;

                }
                else
                {
                    bool alreadyContain = false;
                    var photo = galleryDbContext.Photographies.FirstOrDefault(r => r.PhotographyId == photographyid);
                    if (photo.Locations != null)
                    {
                        foreach (var loc in entity.Locations)
                        {
                            if (photo.Locations.FirstOrDefault(r => r.Id == loc.Id) != null)
                            {
                                alreadyContain = true;
                            }
                        }
                    }
                    if (alreadyContain == true)
                    {
                        return false;
                    }
                    else
                    {
                        entity.Name = item.Name;
                    
                        galleryDbContext.SaveChanges();
                        return true;
                    }


                }


            }
        }
        public List<PhotographyListModel> GetAllFromPerson(ICollection<Location> loc)
        {
            ICollection<Photography> Photographies = new List<Photography>();
            using (var galleryDbContext = new GalleryDbContext())
            {
                foreach (var Location in loc)
                {
                    foreach (var photo in galleryDbContext.Photographies)
                    {
                        if (photo.Locations.FirstOrDefault(r => r.Id == Location.Id) != null)
                        {
                            Photographies.Add(photo);
                        }
                    }
                }
                return Photographies.Select(mapper.MapEntityToPhotographyListModel).ToList();
            }
        }

        public void DeletePerson(Guid id)
        {
            using (var galleryDbContext = new GalleryDbContext())
            {

                var locations = galleryDbContext.Persons.FirstOrDefault(r => r.Id == id).Locations.ToList();
                foreach (var loc in locations)
                {
                    galleryDbContext.Locations.Remove(loc);
                }
                var person = galleryDbContext.Persons.FirstOrDefault(r => r.Id == id);
                galleryDbContext.Persons.Remove(person);
                galleryDbContext.SaveChanges();
            }
        }




    }
}

