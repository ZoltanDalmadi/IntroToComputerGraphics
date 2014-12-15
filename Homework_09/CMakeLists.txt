add_executable("homework_09"
    main.cpp
)

SET_TARGET_PROPERTIES("homework_09"
    PROPERTIES COMPILE_FLAGS
    "-std=c++11 -Wall -pedantic"
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/dist
)

target_link_libraries("homework_09" ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
