using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Client.Helpers
{
    public class RepositoryInMemory : IRepository
    {
        public List<Cluster> GetClusters()
        {
            return new List<Cluster> {
                new Cluster { Guid=new Guid(), Name="1", Caption="11" },
                new Cluster { Guid=new Guid(), Name="2", Caption="22" },
                new Cluster { Guid=new Guid(), Name="3", Caption="33" },
                new Cluster { Guid=new Guid(), Name="4", Caption="44" },
                new Cluster { Guid=new Guid(), Name="5", Caption="55" },
                new Cluster { Guid=new Guid(), Name="6", Caption="66" }
            };
        }
    }
}
