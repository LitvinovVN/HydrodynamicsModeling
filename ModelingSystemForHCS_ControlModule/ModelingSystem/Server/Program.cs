using Microsoft.AspNetCore.ResponseCompression;
using Microsoft.EntityFrameworkCore;
using ModelingSystem.Server;
using ModelingSystem.Server.Services;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
var connectionStringMySql = builder.Configuration.GetConnectionString("mysql_connection");
var serverVersion = new MySqlServerVersion(new Version(8, 0, 27));
builder.Services.AddDbContext<AppDbContextMysql>(options =>
    options.UseMySql(connectionStringMySql, serverVersion));

builder.Services.AddControllersWithViews();
builder.Services.AddRazorPages();

var app = builder.Build();

//DatabaseManagementService.MigrationInitialisation(app);

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseWebAssemblyDebugging();
}
else
{
    app.UseExceptionHandler("/Error");
}

app.UseBlazorFrameworkFiles();
app.UseStaticFiles();

app.UseRouting();


app.MapRazorPages();
app.MapControllers();
app.MapFallbackToFile("index.html");

app.Run();
