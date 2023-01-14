CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(blobrunner)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}  PRIVATE blobrunner.c)


