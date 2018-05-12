using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class ChangeTabItemMessage
    {
        public ChangeTabItemMessage(int changer)
        {
            Changer = changer;
        }
        public int Changer { get; set; }

    }
}
