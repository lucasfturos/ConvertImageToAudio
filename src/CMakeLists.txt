# -------------------------------------------------------------------------

# List all the header files in the src directory
set(SRC_HEADERS
    AlsaPlayer/alsa_player.hpp
    CApp/capp.hpp
    ImageData/img_data.hpp
    ImageToAudio/img_audio.hpp
)

# List all the source files in the src directory
set(SRC_SOURCES
    AlsaPlayer/alsa_player.cpp
    CApp/capp.cpp
    ImageData/img_data.cpp
    ImageToAudio/img_audio.cpp
    main.cpp
)

# Find and configure required libraries
find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)
find_package(PkgConfig REQUIRED)

# PkgConfig check modules
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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ 
)

# Specify libraries or flags to use when linking
target_link_libraries(${PROJECT_NAME}
    PRIVATE ALSA::ALSA
    PRIVATE Threads::Threads
    PRIVATE ${LIBSNDFILE_LIBRARIES}
)

# -------------------------------------------------------------------------