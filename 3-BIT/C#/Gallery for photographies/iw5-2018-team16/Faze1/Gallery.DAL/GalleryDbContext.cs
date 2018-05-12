using Gallery.DAL.Entities;
using System;
using System.Data.Entity;

namespace Faze1
{
    public class GalleryDbContext: DbContext
    {
        public GalleryDbContext(): base("name=GalleryContext")
        {

        }

        public DbSet<Photography> Photographies { get; set; }
        public DbSet<Person> Persons { get; set; }
        public DbSet<Item> Items { get; set; }
        public DbSet<Album> Albums { get; set; } 
        public DbSet<Location> Locations { get; set; }

    }

}
