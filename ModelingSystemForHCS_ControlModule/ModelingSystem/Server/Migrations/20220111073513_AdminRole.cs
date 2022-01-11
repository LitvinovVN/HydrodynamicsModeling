using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ModelingSystem.Server.Migrations
{
    public partial class AdminRole : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.Sql(@"INSERT INTO AspNetRoles (Id, Name, NormalizedName) VALUES ('"+ Guid.NewGuid() +"', 'Admin', 'Admin');");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {

        }
    }
}
