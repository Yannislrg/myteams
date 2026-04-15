cmake_minimum_required(VERSION 3.28)

project(myteams)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MYTEAMS_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/myteams)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

if(APPLE)
  set(MYTEAMS_RUNTIME_LIB_DIR "@executable_path/libs/myteams")
else()
  set(MYTEAMS_RUNTIME_LIB_DIR "$ORIGIN/libs/myteams")
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
  server/commands/use/use.cpp
  server/commands/info/info.cpp
  server/commands/subscribe/subscribe.cpp
  server/commands/unsubscribe/unsubscribe.cpp
)
add_executable(myteams_server ${SERVER_SRC})
target_include_directories(myteams_server PRIVATE
  ${COMMON_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/server
  ${CMAKE_SOURCE_DIR}/server/commands
  ${CMAKE_SOURCE_DIR}/server/database
  ${CMAKE_SOURCE_DIR}/server/client
  ${CMAKE_SOURCE_DIR}/server/models
  ${CMAKE_SOURCE_DIR}/server/net
  ${CMAKE_SOURCE_DIR}/server/sys
  ${CMAKE_SOURCE_DIR}/server/saveManager

  ${MYTEAMS_LIB_DIR}
)
target_compile_options(myteams_server PRIVATE ${COMMON_COMPILER_FLAGS})
if(APPLE)
  target_link_libraries(myteams_server PRIVATE
    ${MYTEAMS_LIB_DIR}/libmyteams_macos.so
  )
  set_target_properties(myteams_server PROPERTIES
    BUILD_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
    INSTALL_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
  )
  add_custom_command(TARGET myteams_server POST_BUILD
    COMMAND install_name_tool -change libmyteams_macos.so @rpath/libmyteams_macos.so $<TARGET_FILE:myteams_server>
  )
else()
  target_link_directories(myteams_server PRIVATE
    ${MYTEAMS_LIB_DIR}
  )
  target_link_libraries(myteams_server PRIVATE
    myteams
    uuid
  )
  set_target_properties(myteams_server PROPERTIES
    SKIP_BUILD_RPATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
  )
endif()


set(CLIENT_SRC
  client/ClientApplication.cpp
  client/main.cpp
  client/network/Poller.cpp
  client/protocol/CommandLineDispatcher.cpp
  client/protocol/TokenParser.cpp
  client/protocol/ServerMessageRouter.cpp
  client/protocol/handlers/TeamCreatedHandler.cpp
  client/protocol/handlers/ChannelCreatedHandler.cpp
  client/protocol/handlers/ThreadCreatedHandler.cpp
  client/protocol/handlers/ReplyCreatedHandler.cpp
  client/protocol/handlers/UserLoggedInHandler.cpp
  client/protocol/handlers/UserLoggedOutHandler.cpp
  client/protocol/handlers/PrivateMessageHandler.cpp
  client/network/TcpClient.cpp
  client/sys/ClientError.cpp
  client/sys/Posix.cpp
)
add_executable(myteams_cli ${CLIENT_SRC})
target_include_directories(myteams_cli PRIVATE
  ${COMMON_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/client
  ${CMAKE_SOURCE_DIR}/client/network
  ${CMAKE_SOURCE_DIR}/client/sys
  ${CMAKE_SOURCE_DIR}/client/protocol
  ${CMAKE_SOURCE_DIR}/client/protocol/handlers
  ${MYTEAMS_LIB_DIR}
)
target_compile_options(myteams_cli PRIVATE ${COMMON_COMPILER_FLAGS})
if(APPLE)
  target_link_libraries(myteams_cli PRIVATE
    ${MYTEAMS_LIB_DIR}/libmyteams_macos.so
  )
  set_target_properties(myteams_cli PROPERTIES
    BUILD_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
    INSTALL_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
  )
  add_custom_command(TARGET myteams_cli POST_BUILD
    COMMAND install_name_tool -change libmyteams_macos.so @rpath/libmyteams_macos.so $<TARGET_FILE:myteams_cli>
  )
else()
  target_link_directories(myteams_cli PRIVATE
    ${MYTEAMS_LIB_DIR}
  )
  target_link_libraries(myteams_cli PRIVATE
    myteams
  )
  set_target_properties(myteams_cli PROPERTIES
    SKIP_BUILD_RPATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${MYTEAMS_RUNTIME_LIB_DIR}"
  )
endif()

option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
