﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL.Messages
{
    public class ChangeVisibilityMessage
    {
        public ChangeVisibilityMessage(bool changer)
        {
            Changer = changer;
        }
        public bool Changer { get; set; }

    }
}
