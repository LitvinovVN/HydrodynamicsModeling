using ModelingSystem.Client.Helpers;
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

        public async Task<int> CreateCluster(Cluster cluster)
        {
            var response = await httpService.Post<Cluster,int>(url, cluster);
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }

            return response.Response;
        }
    }
}
