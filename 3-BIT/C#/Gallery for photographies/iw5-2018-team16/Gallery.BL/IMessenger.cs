﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.BL
{
    public interface IMessenger
    {
        void Register<TMessage>(Action<TMessage> action);
        void Send<TMessage>(TMessage message);
        void UnRegister<TMessage>(Action<TMessage> action);
    }
}
