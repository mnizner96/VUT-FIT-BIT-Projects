using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{

    public class FilterByDateMessage
    {
        public FilterByDateMessage(string time)
        {
            Time = time;
        }

        public string Time { get; set; }
    }
}
