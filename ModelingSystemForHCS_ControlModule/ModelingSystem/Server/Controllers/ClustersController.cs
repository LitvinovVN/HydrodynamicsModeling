using Microsoft.AspNetCore.Mvc;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Server.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ClustersController : ControllerBase
    {
        private readonly AppDbContextMysql context;

        public ClustersController(AppDbContextMysql context)
        {
            this.context = context;
        }

        [HttpPost]
        public async Task<ActionResult<int>> Post(Cluster cluster)
        {            
            context.Add(cluster);
            await context.SaveChangesAsync();
            return cluster.ClusterId;
        }
    }
}
