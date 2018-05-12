using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class SelectedAlbumForSavePhoto
    {
        public SelectedAlbumForSavePhoto(Guid id)
        {
            Id = id;
        }
        public Guid Id { get; set; }

    }
}
