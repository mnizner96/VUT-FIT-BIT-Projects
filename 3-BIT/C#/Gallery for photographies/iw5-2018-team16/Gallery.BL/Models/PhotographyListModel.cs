using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace Gallery.BL.Models
{
   public class PhotographyListModel
    {
        public Guid Id { get; set; }
        public BitmapImage Image { get; set; }
    }
}
