using System;
using Faze1;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using Gallery.DAL.Entities;

namespace Gallery.BL.Models
{
    public class PhotographyDetailModel
    {
        public BitmapImage Image{ get; set; }
        public Guid Id { get; set; }
        public string Name { get; set; }
        public string Time { get; set; }
        public string Format { get; set; }
        public int Height { get; set; }
        public int Weight { get; set; }
        public string Additional { get; set; }
        public Album Album { get; set; }
        public virtual ICollection<Location> Locations { get; set; }

    }
}
