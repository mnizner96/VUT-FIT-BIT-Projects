using Gallery.BL.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
   public class AddToAlbumMessage 
    {
        public AddToAlbumMessage(PhotographyDetailModel model)
        {
            Model = model;
        }
        public PhotographyDetailModel Model;
    }
        
}
