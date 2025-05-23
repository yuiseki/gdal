cmake_minimum_required(VERSION 3.16)

project(test_cpp CXX)

find_package(GDAL CONFIG REQUIRED)

add_executable(test_cpp test_cpp.cpp)
set_property(TARGET test_cpp PROPERTY CXX_STANDARD 11)
target_link_libraries(test_cpp PRIVATE GDAL::GDAL)

option(USE_ODBC "Whether to detect ODBC" ON)
if( USE_ODBC )
    # This test needs `sql.h` from ODBC. ODBC will not become a general dependency of GDAL::GDAL.
    find_package(ODBC)
    if (ODBC_FOUND)
      target_compile_definitions(test_cpp PRIVATE USE_ODBC)
      target_link_libraries(test_cpp PRIVATE ODBC::ODBC)
    endif ()
endif ()
