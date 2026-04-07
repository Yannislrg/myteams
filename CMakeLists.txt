cmake_minimum_required(VERSION 3.28)

project(myteams)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()


set(COMMON_COMPILER_FLAGS -Wall -Wextra -Werror)
set(COMMON_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)


set(SERVER_SRC
  server/main.cpp
  server/server.cpp
  server/net/Poller.cpp
  server/sys/Errors.cpp
  server/sys/Posix.cpp
  server/client/client.cpp
  server/database/Database.cpp
  server/saveManager/saveManager.cpp
  server/commands/commandHandling/commandHandling.cpp
  server/commands/create/create.cpp
  server/commands/list/list.cpp
  server/models/User.cpp
  server/models/Team.cpp
  server/models/Channel.cpp
  server/models/Thread.cpp
  server/models/Reply.cpp
  server/models/Message.cpp
)
add_executable(myteams_server ${SERVER_SRC})
target_include_directories(myteams_server PRIVATE
  ${COMMON_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/server
  ${CMAKE_SOURCE_DIR}/server/libs
)
target_compile_options(myteams_server PRIVATE ${COMMON_COMPILER_FLAGS})
if(APPLE)
  target_link_libraries(myteams_server PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/libmyteams_macos.so
  )
  set_target_properties(myteams_server PROPERTIES
    BUILD_RPATH "@executable_path/libs"
    INSTALL_RPATH "@executable_path/libs"
  )
  add_custom_command(TARGET myteams_server POST_BUILD
    COMMAND install_name_tool -change libmyteams_macos.so @rpath/libmyteams_macos.so $<TARGET_FILE:myteams_server>
  )
else()
  target_link_libraries(myteams_server PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/libmyteams.so
    uuid
  )
endif()


set(CLIENT_SRC client/main.cpp)
add_executable(myteams_cli ${CLIENT_SRC})
target_include_directories(myteams_cli PRIVATE ${COMMON_INCLUDE_DIRS})
target_compile_options(myteams_cli PRIVATE ${COMMON_COMPILER_FLAGS})

option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
