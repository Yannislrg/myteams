cmake_minimum_required(VERSION 3.28)

project(myteams)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MYTEAMS_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/myteams)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "")
set(CMAKE_SKIP_INSTALL_RPATH TRUE)

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
  server/saveManager/JsonParser.cpp
  server/saveManager/JsonSerializer.cpp
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
  server/commands/subscribed/subscribed.cpp
  server/utils/utils.cpp
  server/commands/login/login.cpp
  server/commands/logout/logout.cpp
  server/commands/send/send.cpp
  server/commands/messages/messages.cpp
  server/commands/user/user.cpp
  server/commands/users/users.cpp
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
  ${CMAKE_SOURCE_DIR}/server/utils


  ${MYTEAMS_LIB_DIR}
)
target_compile_options(myteams_server PRIVATE ${COMMON_COMPILER_FLAGS})
target_link_directories(myteams_server PRIVATE ${MYTEAMS_LIB_DIR})
if(APPLE)
  target_link_libraries(myteams_server PRIVATE myteams_macos)
else()
  target_link_libraries(myteams_server PRIVATE myteams uuid)
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
target_link_directories(myteams_cli PRIVATE ${MYTEAMS_LIB_DIR})
if(APPLE)
  target_link_libraries(myteams_cli PRIVATE myteams_macos)
else()
  target_link_libraries(myteams_cli PRIVATE myteams)
endif()

option(WITH_TESTS "Build unit tests" OFF)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
