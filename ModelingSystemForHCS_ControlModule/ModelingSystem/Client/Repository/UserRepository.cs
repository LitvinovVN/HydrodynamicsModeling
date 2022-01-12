using ModelingSystem.Client.Helpers;
using ModelingSystem.Shared.DTOs;

namespace ModelingSystem.Client.Repository
{
    public class UserRepository: IUserRepository
    {
        private readonly IHttpService httpService;
        private readonly string url = "api/users";

        public UserRepository(IHttpService httpService)
        {
            this.httpService = httpService;
        }

        public async Task<PaginatedResponse<List<UserDTO>>> GetUsers(PaginationDTO paginationDTO)
        {
            return await httpService.GetHelper<List<UserDTO>>(url, paginationDTO);
        }

        public async Task<List<RoleDTO>> GetRoles()
        {
            return await httpService.GetHelper<List<RoleDTO>>($"{url}/roles");
        }

        public async Task AssignRole(EditRoleDTO editRoleDTO)
        {
            var response = await httpService.Post($"{url}/assignRole", editRoleDTO);
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }
        }

        public async Task RemoveRole(EditRoleDTO editRoleDTO)
        {
            var response = await httpService.Post($"{url}/removeRole", editRoleDTO);
            if (!response.Success)
            {
                throw new ApplicationException(await response.GetBody());
            }
        }
    }
}
