namespace Faze1
{
    using Gallery.DAL.Entities;
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;

    public class Item
    {
        [Key]
        public Guid Id { get; set; }
        public string Name { get; set; }
        public virtual ICollection<Location> Locations { get; set; } = new List<Location>();
    }
}
