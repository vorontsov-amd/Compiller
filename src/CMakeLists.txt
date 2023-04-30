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

add_executable(${PROJECT_NAME} ${CPP_SOURCES})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ./include)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME})