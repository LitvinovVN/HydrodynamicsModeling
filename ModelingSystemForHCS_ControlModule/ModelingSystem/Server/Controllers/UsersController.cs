using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ModelingSystem.Server.Helpers;
using ModelingSystem.Shared.DTOs;
using System.Security.Claims;

namespace ModelingSystem.Server.Controllers
{
    [ApiController]
    [Route("/api/[controller]")]
    public class UsersController : ControllerBase
    {
        private readonly AppDbContextMysql appDbContextMysql;
        private readonly UserManager<IdentityUser> userManager;

        public UsersController(AppDbContextMysql appDbContextMysql,
            UserManager<IdentityUser> userManager)
        {
            this.appDbContextMysql = appDbContextMysql;
            this.userManager = userManager;
        }

        [HttpGet]
        public async Task<ActionResult<List<UserDTO>>> Get([FromQuery] PaginationDTO paginationDTO)
        {
            var queryable = appDbContextMysql.Users.AsQueryable();
            await HttpContext.InsertPaginationParametersInResponse(queryable, paginationDTO.RecordsPerPage);
            return await queryable.Paginate(paginationDTO)
                .Select(x => new UserDTO { Email = x.Email, UserId = x.Id })
                .ToListAsync();
        }

        [HttpGet("roles")]        
        public async Task<ActionResult<List<RoleDTO>>> Get()
        {
            return await appDbContextMysql.Roles
                .Select(x => new RoleDTO { RoleName = x.Name })
                .ToListAsync();
        }

        [HttpPost("assignRole")]
        public async Task<ActionResult> AssignRole(EditRoleDTO editRoleDTO)
        {
            var user = await userManager.FindByIdAsync(editRoleDTO.UserId);
            await userManager.AddClaimAsync(user, new Claim(ClaimTypes.Role, editRoleDTO.RoleName));
            return NoContent();
        }

        [HttpPost("removeRole")]
        public async Task<ActionResult> RemoveRole(EditRoleDTO editRoleDTO)
        {
            var user = await userManager.FindByIdAsync(editRoleDTO.UserId);
            await userManager.RemoveClaimAsync(user, new Claim(ClaimTypes.Role, editRoleDTO.RoleName));
            return NoContent();
        }
    }
}
