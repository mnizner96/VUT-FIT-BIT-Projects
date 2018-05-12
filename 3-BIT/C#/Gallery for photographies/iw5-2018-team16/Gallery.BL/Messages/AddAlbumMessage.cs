using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
   public class AddAlbumMessage 
    {
        public AddAlbumMessage(string name)
        {
            Name = name;
        }
        string Name;
    }
}
