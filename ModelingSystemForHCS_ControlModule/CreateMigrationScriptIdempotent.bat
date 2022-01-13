cd ModelingSystem\Server
dotnet ef migrations script --idempotent -o ..\..\db\ModelingSystem-mysql-migrations\migrateModelingSystem.sql

cd ..\..
cd BlazorAuthentication\Server
dotnet ef migrations script --idempotent -o ..\..\db\ModelingSystem-mysql-migrations\migrateBlazorAuthentication.sql