using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Faze1
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;

    public class Album
    {
        [Required]
        [Key]
        public Guid Id { get; set; }
        [Required]
        public string Name { get; set; }
        public virtual ICollection<Photography> Photographies { get; set; } = new List<Photography>();
    }
}

