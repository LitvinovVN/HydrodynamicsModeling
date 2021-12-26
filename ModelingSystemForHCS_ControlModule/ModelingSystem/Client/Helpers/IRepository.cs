using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Helpers
{
    public interface IRepository
    {
        public List<Cluster> GetClusters();
    }
}
