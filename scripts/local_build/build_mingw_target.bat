if not defined GMIO_BUILD_ROOT (
    echo "****ERROR: Variable GMIO_BUILD_ROOT not defined"
    exit 0
)

set BUILD_TYPE=%1
set BUILD_OUTPUT_FILE=%GMIO_BUILD_ROOT%\%CURRENT_BUILD%_%BUILD_TYPE%.txt 2>&1
cd %GMIO_BUILD_ROOT%\%CURRENT_BUILD%\%BUILD_TYPE%
cmake %GMIO_ROOT% -G "MinGW Makefiles" ^
      -DCMAKE_BUILD_TYPE=%BUILD_TYPE%  -DCMAKE_DEBUG_POSTFIX=_d ^
      -DCMAKE_INSTALL_PREFIX=%GMIO_BUILD_ROOT%\%CURRENT_BUILD%\install ^
      -DGMIO_FLOAT2STR_LIB=google_doubleconversion ^
      -DGMIO_BUILD_EXAMPLES=ON ^
      -DGMIO_BUILD_TESTS_FAKE_SUPPORT=ON ^
      -DGMIO_BUILD_BENCHMARKS=ON ^
      -DGMIO_BUILD_BENCHMARK_ASSIMP=OFF ^
      -DGMIO_BUILD_BENCHMARK_OPENCASCADE=OFF ^
      -DGMIO_BUILD_BENCHMARK_LIB3MF=OFF ^
      > %BUILD_OUTPUT_FILE%
mingw32-make -j4 >> %BUILD_OUTPUT_FILE%
mingw32-make check >> %BUILD_OUTPUT_FILE%
mingw32-make install >> %BUILD_OUTPUT_FILE%
