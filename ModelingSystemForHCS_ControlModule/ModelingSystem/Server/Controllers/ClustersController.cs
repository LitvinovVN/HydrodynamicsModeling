using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ModelingSystem.Server.Helpers;
using ModelingSystem.Shared.DTOs;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Server.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    [Authorize(AuthenticationSchemes = JwtBearerDefaults.AuthenticationScheme, Roles = "Admin")]
    public class ClustersController : ControllerBase
    {
        private readonly AppDbContextMysql context;

        public ClustersController(AppDbContextMysql context)
        {
            this.context = context;
        }

        [HttpGet]
        [AllowAnonymous]
        public async Task<ActionResult<List<Cluster>>> Get([FromQuery]PaginationDTO paginationDTO)
        {
            var queryable = context.Clusters.AsQueryable();
            await HttpContext.InsertPaginationParametersInResponse(queryable, paginationDTO.RecordsPerPage);

            return await queryable.Paginate(paginationDTO).ToListAsync();
        }

        [HttpGet("{id}")]
        [AllowAnonymous]
        public async Task<ActionResult<Cluster>> Get(int id)
        {
            var cluster = await context.Clusters.FindAsync(id);
            if (cluster == null) return NotFound();

            return cluster;
        }

        [HttpGet("search/{searchText}")]
        [AllowAnonymous]
        public async Task<ActionResult<List<Cluster>>> FilterByName(string searchText)
        {
            if(string.IsNullOrWhiteSpace(searchText))
            {
                return new List<Cluster>();
            }

            return await context.Clusters
                .Where(x => x.Name.Contains(searchText))
                .ToListAsync();
        }

        [HttpPost]
        public async Task<ActionResult<int>> Post(Cluster cluster)
        {            
            context.Add(cluster);
            await context.SaveChangesAsync();
            return cluster.ClusterId;
        }

        [HttpPost("filter")]
        public async Task<ActionResult<List<Cluster>>> Filter(FilterClustersDTO filterClustersDTO)
        {
            var clustersQueryable = context.Clusters.AsQueryable();
            if(!string.IsNullOrWhiteSpace(filterClustersDTO.Name))
            {
                clustersQueryable = clustersQueryable.Where(x => x.Name.Contains(filterClustersDTO.Name));
            }

            await HttpContext.InsertPaginationParametersInResponse(clustersQueryable,
                filterClustersDTO.RecordsPerPage);

            var clusters = await clustersQueryable.Paginate(filterClustersDTO.Pagination).ToListAsync();

            return clusters;
        }

        [HttpPut]
        public async Task<ActionResult> Put(Cluster cluster)
        {
            context.Attach(cluster).State = EntityState.Modified;
            await context.SaveChangesAsync();
            return NoContent();
        }

        [HttpDelete("{id}")]
        public async Task<ActionResult> Delete(int id)
        {
            var entry = await context.Clusters.FindAsync(id);
            if (entry == null) return NotFound();
            context.Remove(entry);
            await context.SaveChangesAsync();
            return NoContent();
        }

    }
}
