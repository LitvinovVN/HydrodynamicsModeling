using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModelingSystem.Shared.DTOs
{
    public class FilterClustersDTO
    {
        public int Page { get; set; } = 1;
        public int RecordsPerPage { get; set; } = 10;

        public PaginationDTO Pagination
        {
            get
            {
                return new PaginationDTO { Page = Page, RecordsPerPage = RecordsPerPage };
            }
        }

        public string Name { get; set; } = "";
    }
}
