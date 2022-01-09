namespace ModelingSystem.Client.Auth
{
    public interface ILoginService
    {
        Task Login(string token);
        Task Logout();
    }
}
