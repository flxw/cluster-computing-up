
cmake_minimum_required(VERSION 3.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(montecarlo_pi montecarlo_pi.c timing_utils.c)
target_link_libraries(montecarlo_pi pthread m)

# add_executable(affine_montecarlo_pi affine_montecarlo_pi.c)
# target_link_libraries(affine_montecarlo_pi pthread m)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(omp_montecarlo_pi omp_montecarlo_pi.c timing_utils.c)
target_link_libraries(omp_montecarlo_pi)