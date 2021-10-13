git pull

cat description.md
echo ''
echo '------- Compiling MatmAlgsTesting.cpp into MatmAlgsTesting.o: mpicxx -O3 -c MatmAlgsTesting.cpp ------'
mpicxx --std=c++11 -O3 -c Grid3D.cpp
mpicxx --std=c++11 -O3 -c main.cpp
echo '-------- mpicxx -o myapp MatmAlgsTesting.o -----'
mpicxx -o myapp Grid3D.o main.o
echo '-------------'

echo '------- Starting myapp in 1 node with 1 cpu per node with maxtime by default (5 minutes) ------'
echo '------- mpirun -np 1 ./myapp --------'
mpirun -np 1 ./myapp

echo '------- Task list: mps ---------'
mps

echo '------- Task list: pult t ---------'
pult t

echo '------- Status of executed task: mqtest myapp.1 --------'
mqtest myapp.1
echo '------- End of run.sh ------'