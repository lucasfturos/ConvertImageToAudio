# -------------------------------------------------------------------------

# List all the header files in the src directory
set(SRC_HEADERS
    common/util.hpp
    ToneGenerate/tone_gen.hpp
)

# List all the source files in the src directory
set(SRC_SOURCES
    ToneGenerate/tone_gen.cpp
    main.cpp
)

# Find and configure required libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSNDFILE REQUIRED sndfile)

# Add executable target with source and header files
add_executable(${PROJECT_NAME} 
    ${SRC_HEADERS} 
    ${SRC_SOURCES}
)

# Add include directories and libraries for LIBSNDFILE
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
link_directories(${LIBSNDFILE_LIBRARY_DIRS})

# Specify target-specific include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE src 
)

# Specify libraries or flags to use when linking
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${LIBSNDFILE_LIBRARIES}
)

# -------------------------------------------------------------------------