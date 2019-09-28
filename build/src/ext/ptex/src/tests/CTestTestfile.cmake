# CMake generated Testfile for 
# Source directory: /Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex/src/tests
# Build directory: /Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(wtest "/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/wtest")
add_test(rtest "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake" "-DOUT=/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/rtest.out" "-DDATA=/Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex/src/tests/rtestok.dat" "-DCMD=/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/rtest" "-P" "/Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex/src/tests/compare_test.cmake")
add_test(ftest "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake" "-DOUT=/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/ftest.out" "-DDATA=/Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex/src/tests/ftestok.dat" "-DCMD=/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/ftest" "-P" "/Users/valentin/Desktop/Workspace/pbrt/src/ext/ptex/src/tests/compare_test.cmake")
add_test(halftest "/Users/valentin/Desktop/Workspace/pbrt/build/src/ext/ptex/src/tests/halftest")
