cmake -G Ninja -B build
cmake --build ./build --config release
cd build/test
jsonMaster-bench.exe
cd ../..
pause
