using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Repository
{
    public interface IClusterRepository
    {
        Task<int> CreateCluster(Cluster cluster);
    }
}
