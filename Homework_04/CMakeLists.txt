add_executable("homework_04"
    main.cpp
)

SET_TARGET_PROPERTIES("homework_04"
    PROPERTIES COMPILE_FLAGS
    "-std=c++11 -Wall -pedantic"
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/dist
)

target_link_libraries("homework_04" ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
