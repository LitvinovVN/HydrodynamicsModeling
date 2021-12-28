using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelingSystem.Shared.Entities
{
    /// <summary>
    /// Вычислительный кластер
    /// </summary>
    public class Cluster
    {
        //tex:
        //$$a=b + \alpha\tau^2$$
        public Guid Guid { get; set; }

        /// <summary>
        /// Наименование кластера
        /// </summary>
        [Required(ErrorMessage = "Введите наименование кластера!")]
        public string Name { get; set; }

        public string Caption { get; set; }
    }
}
