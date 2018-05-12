using Gallery.DAL.Entities.Base.Interface;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gallery.DAL.Entities.Base.Implementation
{
   public abstract class EntityBase : IEntity
    {
        public Guid Id { get; set; } = Guid.NewGuid();
    }
}
