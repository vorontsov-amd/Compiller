set(CPP_SOURCES
    Compiller.cpp
    data_t.cpp
    Differentiator.cpp
    lexical_analysis.cpp
    main.cpp
    Reading.cpp
)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(${PROJECT_NAME} ${CPP_SOURCES})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ./include)

target_link_libraries(${PROJECT_NAME} PRIVATE LLVM)

add_subdirectory(${PROJECT_SOURCE_DIR}/termcolor ${CMAKE_CURRENT_BINARY_DIR}/termcolors)
target_link_libraries(${PROJECT_NAME} PRIVATE termcolor)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})