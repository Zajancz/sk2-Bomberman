cmake_minimum_required(VERSION 3.10)

# Set the project name
project(my_project)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# Add the source files for the server executable
add_executable(server
  src/server/Main.cpp
)

# Add the source files for the client executable
add_executable(client
  src/client/Main.cpp
)

# Add the necessary include directories for the project
target_include_directories(server PRIVATE src/server)
target_include_directories(client PRIVATE src/client)

# Add the necessary libraries for the project
target_link_libraries(server PRIVATE stdc++ pthread)
target_link_libraries(client PRIVATE stdc++ pthread)
