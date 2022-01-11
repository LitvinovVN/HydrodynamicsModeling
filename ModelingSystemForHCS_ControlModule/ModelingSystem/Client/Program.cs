using Microsoft.AspNetCore.Components.Authorization;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using ModelingSystem.Client;
using ModelingSystem.Client.Auth;
using ModelingSystem.Client.Helpers;
using ModelingSystem.Client.Repository;

var builder = WebAssemblyHostBuilder.CreateDefault(args);
builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

builder.Services.AddSingleton(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });
builder.Services.AddTransient<IRepository, RepositoryInMemory>();
builder.Services.AddScoped<IHttpService, HttpService>();
builder.Services.AddScoped<IClusterRepository, ClusterRepository>();
builder.Services.AddScoped<IAccountsRepository, AccountsRepository>();
builder.Services.AddScoped<IDisplayMessage, DisplayMessage>();

builder.Services.AddAuthorizationCore();

builder.Services.AddScoped<JwtAuthenticationStateProvider>();
builder.Services.AddScoped<AuthenticationStateProvider, JwtAuthenticationStateProvider>(
    provider => provider.GetRequiredService<JwtAuthenticationStateProvider>());
builder.Services.AddScoped<ILoginService, JwtAuthenticationStateProvider>(
    provider => provider.GetRequiredService<JwtAuthenticationStateProvider>());

await builder.Build().RunAsync();
