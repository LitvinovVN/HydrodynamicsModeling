using Microsoft.AspNetCore.Components.Authorization;
using System.Security.Claims;

namespace ModelingSystem.Client.Auth
{
    public class DummyAuthenticationStateProvider : AuthenticationStateProvider
    {
        public override async Task<AuthenticationState> GetAuthenticationStateAsync()
        {
            //await Task.Delay(3000);
            //var anonymous = new ClaimsIdentity(); // не авторизовано
            //var anonymous = new ClaimsIdentity("test"); // авторизовано
            var anonymous = new ClaimsIdentity(new List<Claim>()
                {
                    new Claim("key1", "value 1"),
                    new Claim(ClaimTypes.Name, "Владимир"),
                    new Claim(ClaimTypes.Role, "Admin")
                },"");
            return await Task.FromResult(new AuthenticationState(new ClaimsPrincipal(anonymous)));
        }
    }
}
