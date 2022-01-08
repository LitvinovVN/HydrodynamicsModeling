using ModelingSystem.Shared.DTOs;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Repository
{
    public interface IClusterRepository
    {
        Task<int> CreateCluster(Cluster cluster);
        Task<Cluster> GetCluster(int id);
        Task<List<Cluster>> GetClusters();
        Task<List<Cluster>> GetClustersByName(string name);
        Task UpdateCluster(Cluster cluster);
        Task DeleteCluster(int id);
        Task<PaginatedResponse<List<Cluster>>> GetClusters(PaginationDTO paginationDTO);
        Task<PaginatedResponse<List<Cluster>>> GetClustersFiltered(FilterClustersDTO filterClustersDTO);
    }
}
