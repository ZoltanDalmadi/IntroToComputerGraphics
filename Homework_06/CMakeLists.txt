add_executable("homework_06"
    main.cpp
)

SET_TARGET_PROPERTIES("homework_06"
    PROPERTIES COMPILE_FLAGS
    "-std=c++11 -Wall -pedantic"
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/dist
)

target_link_libraries("homework_06" ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
