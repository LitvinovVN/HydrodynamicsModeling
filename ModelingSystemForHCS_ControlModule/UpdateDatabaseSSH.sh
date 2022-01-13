scp.exe "db\ModelingSystem-mysql-migrations\migrate.sql" root@92.63.111.155:~/HydrodynamicsModeling/ModelingSystemForHCS_ControlModule/db/ModelingSystem-mysql-migrations/migrate.sql
scp.exe "scripts\mysql-migrate.sh" root@92.63.111.155:~/HydrodynamicsModeling/ModelingSystemForHCS_ControlModule/scripts/mysql-migrate.sh

ssh root@92.63.111.155

for container_id in $(docker ps  -f "name=mysql" -q);do docker exec -ti $container_id bash;done
mysql -u admin -pPASSWORD ModelingSystem < "/var/mysql-migrations/migrate.sql"
mysql -u root -ppassword BlazorAuthentication < "/var/mysql-migrations/migrateBlazorAuthentication.sql"