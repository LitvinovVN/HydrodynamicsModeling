using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelingSystem.Shared.Entities
{
    public class Cluster
    {
        public Guid Guid { get; set; }

        public string Name { get; set; } = string.Empty;

        public string Caption { get; set; } = string.Empty;
    }
}
