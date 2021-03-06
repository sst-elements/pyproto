cmake_minimum_required(VERSION 3.12)
project(pyproto CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)
set(INCLUDES "")
set(LIBRARIES "")

# -------------------- SST CONFIGS --------------------
execute_process(
    COMMAND which sst-config
    OUTPUT_VARIABLE SST_CONFIG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${SST_CONFIG} --ELEMENT_CXXFLAGS
    OUTPUT_VARIABLE SST_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${SST_CONFIG} --ELEMENT_LDFLAGS
    OUTPUT_VARIABLE SST_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${SST_CONFIG} --PYTHON_CPPFLAGS
    OUTPUT_VARIABLE SST_PYTHON_CPPFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

separate_arguments(SST_FLAGS)
separate_arguments(SST_LDFLAGS)
separate_arguments(SST_PYTHON_CPPFLAGS)
execute_process(
    COMMAND which sst-register
    OUTPUT_VARIABLE SST_REGISTER
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
macro(_sst_compile_link _name)
    if (TARGET ${_name})
        target_compile_definitions(${_name} PRIVATE)
        target_compile_options(${_name} PRIVATE ${SST_FLAGS} ${SST_PYTHON_CPPFLAGS} ${INCLUDES})
        target_link_options(${_name} PRIVATE ${SST_LDFLAGS} ${LIBRARIES})
        execute_process(COMMAND ${SST_REGISTER} ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_LIBDIR=${CMAKE_CURRENT_SOURCE_DIR}/build)
    endif ()
endmacro()

add_custom_target(
    uninstall
    COMMAND ${SST_REGISTER} -u ${CMAKE_PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} --build . --target clean --config $<CONFIG>
)
# -------------------- SST CONFIGS --------------------

# execute command:
# od -v -t x1 < FILE.py | sed -e 's/^[^ ]*[ ]*//g' -e '/^\s*$/d' -e 's/\([0-9a-f]*\)[ $]*/0x\1,/g' > FILE.inc
macro(octal_dump _name)
    execute_process(
            COMMAND od -v -t x1 ${CMAKE_CURRENT_SOURCE_DIR}/${_name}.py
            COMMAND sed -e "s/^[^ ]*[ ]*//g"
            COMMAND sed -e "/^\\s*$/d"
            COMMAND sed -r -e "s/\([0-9a-f]*\)[ $]*/0x\\1,/g"
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${_name}.inc
    )
endmacro()
octal_dump(pyproto)


# -------------------- SST EXECUTABLES --------------------
set(
    SOURCES
    pyarchive.cc
    pymodule.cc
    pyproto.cc
)

add_executable(
    lib${CMAKE_PROJECT_NAME}.so
    ${SOURCES}
)

_sst_compile_link(lib${CMAKE_PROJECT_NAME}.so)
# -------------------- SST EXECUTABLES --------------------
