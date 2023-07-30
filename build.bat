set CommonCompilerFlags=-std=c++17 -m64 -g -Werror -fno-exceptions -fwritable-strings -O0 -Wno-unknown-warning-option -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -DWIN32
set LinkerFlags=-I../src -I../libs/jpeglib/include -L ../libs/jpeglib/lib -ljpeg

xcopy /E /I /D /Y "./libs/jpeglib/lib" "./build"
xcopy /E /I /D /Y "./assets" "./build/assets"

pushd build

clang++ %CommonCompilerFlags% %LinkerFlags% ../src/main.cpp -o ./main.exe