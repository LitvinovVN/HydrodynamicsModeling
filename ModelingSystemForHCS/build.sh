git pull

mkdir _build
cd _build
cmake ../src -DCMAKE_BUILD_TYPE=Release
cmake --build .

./myApp