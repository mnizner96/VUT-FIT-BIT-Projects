namespace Faze1
{
    using Gallery.DAL.Entities;
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.Drawing;
    using System.Windows.Media.Imaging;

    public class Photography
    {
        [Key]
        public Guid PhotographyId { get; set; }
        [Required]
        public byte[] Image { get; set; }

        [Required]
        public string Name { get; set; }

        public string Time { get; set; }
    
        public string Format { get; set; }
        public int Height { get; set; }
        public int Weight { get; set; }
        public string Additional { get; set; }
        public virtual Album Album { get; set; }
        public virtual ICollection<Location> Locations { get; set; } = new List<Location>();
    }
}