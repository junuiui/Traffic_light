# CMake for app
include_directories(include)
file(GLOB MY_SOURCES "src/*.c")

find_package(Threads REQUIRED)
add_executable(traffic_light ${MY_SOURCES})

target_link_libraries(traffic_light LINK_PRIVATE hal Threads::Threads)

add_custom_command(TARGET traffic_light POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:traffic_light>"
        "~/cmpt433/public/traffic_light_dir/traffic_light"   # Change here for your directory
    COMMENT "Copying ARM executable to public NFS directory")
