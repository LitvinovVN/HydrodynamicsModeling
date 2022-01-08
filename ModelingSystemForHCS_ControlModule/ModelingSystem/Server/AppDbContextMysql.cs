using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using ModelingSystem.Shared.Entities;

namespace ModelingSystem.Server
{
    public class AppDbContextMysql : IdentityDbContext
    {
        public AppDbContextMysql(DbContextOptions<AppDbContextMysql> options)
            :base(options)
        {

        }

        public DbSet<Cluster> Clusters { get; set; }
    }
}
