using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class PersonSearchMessage
    {
        public PersonSearchMessage(string name)
        {
            Name = name;
        }

        public string Name { get; set; }
    }
}