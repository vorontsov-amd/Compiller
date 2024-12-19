set(CPP_SOURCES
    ByteArray.cpp
    Compiller.cpp
    data_t.cpp
    Differentiator.cpp
    lexical_analysis.cpp
    main.cpp
    Reading.cpp
    Variable.cpp
)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(${PROJECT_NAME} ${CPP_SOURCES})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ./include)

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM)

add_subdirectory(${PROJECT_SOURCE_DIR}/termcolor ${CMAKE_CURRENT_BINARY_DIR}/termcolors)
target_link_libraries(${PROJECT_NAME} PRIVATE termcolor)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})