project(ir-interpreter)

find_package(SDL2 REQUIRED)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${PROJECT_NAME} ${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} main.cpp
               ../sdl.c)
target_link_libraries(${PROJECT_NAME} PRIVATE LLVM)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})