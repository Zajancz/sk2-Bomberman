cmake_minimum_required(VERSION 3.10)

# Set the project name
project(my_project)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# Set compiler flags
# include ( CheckCXXCompilerFlag )
# foreach ( FLAG -Wall -Wextra )
#     check_cxx_compiler_flag ( "${FLAG}" COMPILER_SUPPORTS_${FLAG} )
#     if ( COMPILER_SUPPORTS_${FLAG} )
#         add_compile_options ( ${FLAG} )
#     endif ( )
# endforeach ( )

# Add the source files for the server executable
file(GLOB server_SRC
     "src/server/*.h"
     "src/server/*.cpp"
)
add_executable(server ${server_SRC})

# Add the source files for the client executable
file(GLOB client_SRC
     "src/client/*.h"
     "src/client/*.cpp"
     "src/server/*.cpp"
     "src/server/*.h"
)
list(FILTER client_SRC EXCLUDE REGEX "src/server/Main\.cpp")
add_executable(client ${client_SRC})

# Add the necessary include directories for the project
target_include_directories(server PRIVATE src/server)
target_include_directories(client PRIVATE src/client)

# Add the necessary libraries for the project
target_link_libraries(server PRIVATE stdc++ pthread)
target_link_libraries(client PRIVATE stdc++ pthread)
