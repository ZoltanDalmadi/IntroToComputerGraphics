add_executable("homework_02"
    main.cpp
)

SET_TARGET_PROPERTIES("homework_02"
    PROPERTIES COMPILE_FLAGS
    "-std=c++11 -Wall -pedantic"
    RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_SOURCE_DIR}/dist
)

target_link_libraries("homework_02" ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
