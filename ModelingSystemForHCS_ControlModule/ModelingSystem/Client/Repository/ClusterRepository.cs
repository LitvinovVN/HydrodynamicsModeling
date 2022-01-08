using ModelingSystem.Client.Helpers;
using ModelingSystem.Shared.DTOs;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Repository
{
    public class ClusterRepository : IClusterRepository
    {
        private readonly IHttpService httpService;
        private string url = "api/clusters";

        public ClusterRepository(IHttpService httpService)
        {
            this.httpService = httpService;
        }

        public async Task<List<Cluster>> GetClusters()
        {
            var response = await httpService.Get<List<Cluster>>(url);
            if (!response.Success)
            {                
                throw new ApplicationException(await response.GetBody());
            }
            return response.Response;
        }

        public async Task<PaginatedResponse<List<Cluster>>> GetClusters(PaginationDTO paginationDTO)
        {
            return await httpService.GetHelper<List<Cluster>>(url, paginationDTO);
        }

        public async Task<List<Cluster>> GetClustersByName(string name)
        {
            var response = await httpService.Get<List<Cluster>>($"{url}/search/{name}");
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }
            return response.Response;
        }

        public async Task<PaginatedResponse<List<Cluster>>> GetClustersFiltered(FilterClustersDTO filterClustersDTO)
        {
            Console.WriteLine("TRACING!");
            var httpResponse = await httpService.Post<FilterClustersDTO, List<Cluster>>($"{url}/filter", filterClustersDTO);
            var totalAmountPages = int.Parse(httpResponse.HttpResponseMessage.Headers.GetValues("totalAmountPages").FirstOrDefault());

            var paginatedResponse = new PaginatedResponse<List<Cluster>>()
            {
                Response = httpResponse.Response,
                TotalAmountPages = totalAmountPages
            };

            return paginatedResponse;
        }

        public async Task<Cluster> GetCluster(int id)
        {
            return await httpService.GetHelper<Cluster>($"{url}/{id}");
        }

        
        public async Task<int> CreateCluster(Cluster cluster)
        {
            var response = await httpService.Post<Cluster,int>(url, cluster);
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }

            return response.Response;
        }

        public async Task UpdateCluster(Cluster cluster)
        {
            var response = await httpService.Put<Cluster>(url, cluster);
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }            
        }

        public async Task DeleteCluster(int id)
        {
            var response = await httpService.Delete($"{url}/{id}");
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }
        }
    }
}
