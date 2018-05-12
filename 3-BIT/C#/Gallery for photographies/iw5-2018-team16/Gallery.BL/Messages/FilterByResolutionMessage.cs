using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{

    public class FilterByResolutionMessage
    {
        public FilterByResolutionMessage(string resolution)
        {
            Resolution = resolution;
        }

        public string Resolution { get; set; }
    }
}
