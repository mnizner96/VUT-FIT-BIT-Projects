using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.DAL.Entities
{
    public class Location
    {
        [Key]
        public Guid Id { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
    }
}
