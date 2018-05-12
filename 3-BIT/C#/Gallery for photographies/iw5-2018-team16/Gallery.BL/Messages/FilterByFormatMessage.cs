using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class FilterByFormatMessage
    {
        public FilterByFormatMessage(string format)
        {
            Format = format;
        }

        public string Format { get; set; }
    }
}