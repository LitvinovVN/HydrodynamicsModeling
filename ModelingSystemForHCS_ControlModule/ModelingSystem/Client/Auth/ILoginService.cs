using ModelingSystem.Shared.DTOs;

namespace ModelingSystem.Client.Auth
{
    public interface ILoginService
    {
        Task Login(UserToken userToken);
        Task Logout();
        Task TryRenewToken();
    }
}
