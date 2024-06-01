cd build
call "D:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake.exe --build . --target learn_2d_game_physics_programming
cd ..
pause
start x64/learn_2d_game_physics_programming.exe
