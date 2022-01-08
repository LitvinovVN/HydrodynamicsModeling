using AutoMapper;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Server.Helpers
{
    public class AutomapperProfiles: Profile
    {
        public AutomapperProfiles()
        {
            CreateMap<Cluster, Cluster>();
        }
    }
}
