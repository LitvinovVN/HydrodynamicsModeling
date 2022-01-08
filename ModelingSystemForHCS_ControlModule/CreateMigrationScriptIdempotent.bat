cd ModelingSystem\Server
dotnet ef migrations script --idempotent -o ..\..\db\ModelingSystem-mysql-migrations\migrate.sql