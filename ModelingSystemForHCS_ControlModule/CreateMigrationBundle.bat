cd ModelingSystem\Server
copy "appsettings.json" "..\..\db\ModelingSystem-mysql-migrations\appsettings.json"
dotnet ef migrations bundle --self-contained -r linux-x64 -o ..\..\db\ModelingSystem-mysql-migrations\efbundle --force