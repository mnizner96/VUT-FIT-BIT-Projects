using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class DeletePhotoMessage
    {
        public DeletePhotoMessage(Guid id)
        {
            Id = id;
        }
        public Guid Id { get; set; }
    }
}
