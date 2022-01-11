#!/bin/bash
for container_id in $(docker ps  -f "name=mysql" -q);do docker exec -ti $container_id bash;done
mysql -u admin -pPASSWORD ModelingSystem < "/var/mysql-migrations/migrate.sql"