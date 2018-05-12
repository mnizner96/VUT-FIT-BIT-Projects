namespace Faze1
{
    using Gallery.DAL.Entities;
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;

    public class Person
    {
        [Key]
        public Guid Id { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public virtual ICollection<Location> Locations { get; set; } = new List<Location>();
    }
}