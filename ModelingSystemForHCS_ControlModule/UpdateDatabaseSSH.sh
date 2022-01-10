scp.exe "db\ModelingSystem-mysql-migrations\migrate.sql" root@92.63.111.155:~/HydrodynamicsModeling/ModelingSystemForHCS_ControlModule/db/ModelingSystem-mysql-migrations/migrate.sql

root@92.63.111.155
docker exec -ti (docker ps -f "name=mysql" --format "{{.ID}}") bash
mysql -u admin -ppassword ModelingSystem < "/var/mysql-migrations/migrate.sql"