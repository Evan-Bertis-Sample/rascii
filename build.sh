# make the build directory
mkdir -p build
# change to the build directory
cd build
# run cmake
cmake .. -G "Unix Makefiles"
# run make
make
