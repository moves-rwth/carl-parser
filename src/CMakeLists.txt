# WARNING:
# add_custom_command makes the output files only visible in the same directory.
# Therefore, we cannot simply run this code here, but must export it as a function to be called in the right context.

function(create_antlr_targets)
        find_package(Java COMPONENTS Runtime)

        set(ANTLR_generated_sources "")
        file(GLOB_RECURSE antlr_grammars "${CMAKE_SOURCE_DIR}/src/*.g4")

        set(ANTLR_generated_include "" PARENT_SCOPE)
        set(ANTLR_generated_sources "" PARENT_SCOPE)

        foreach(file ${antlr_grammars})
                get_filename_component(DIR ${file} DIRECTORY)
                file(RELATIVE_PATH DIR ${CMAKE_SOURCE_DIR}/src "${DIR}")
                get_filename_component(FILENAME ${file} NAME)
                get_filename_component(GRAMMAR ${file} NAME_WE)
                set(DESTDIR "${CMAKE_BINARY_DIR}/src/${DIR}")

                # https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
                set(generated_sources
                        ${DESTDIR}/${GRAMMAR}Lexer.cpp
                        ${DESTDIR}/${GRAMMAR}Parser.cpp
                        ${DESTDIR}/${GRAMMAR}Visitor.cpp
                        ${DESTDIR}/${GRAMMAR}BaseVisitor.cpp
                        ${DESTDIR}/${GRAMMAR}Listener.cpp
                        ${DESTDIR}/${GRAMMAR}BaseListener.cpp
                )
                set(ANTLR_generated_sources "${ANTLR_generated_sources};${generated_sources}")

                add_custom_command(
                        OUTPUT ${generated_sources}
                        COMMAND ${CMAKE_COMMAND} -E make_directory ${DESTDIR}
                        COMMAND ${Java_JAVA_EXECUTABLE} -jar ${ANTLR_JAR} -Werror -Dlanguage=Cpp -listener -visitor -o ${DESTDIR} -package carl ${file}
                        DEPENDS ${file}
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
                )
        endforeach()
        set(ANTLR_generated_include ${ANTLR_generated_include} "${CMAKE_BINARY_DIR}/src" PARENT_SCOPE)
        set(ANTLR_generated_sources ${ANTLR_generated_sources} "${generated_sources}" PARENT_SCOPE)
endfunction()

create_antlr_targets()

add_library(carl-parser SHARED "${CMAKE_CURRENT_SOURCE_DIR}/ParseTreeVisitor.cpp" ${ANTLR_generated_sources})
target_include_directories(carl-parser PUBLIC ${ANTLR_generated_include})
target_link_libraries(carl-parser PUBLIC antlr4-shared)
target_link_libraries(carl-parser PUBLIC lib_carl)
add_dependencies(lib_carl ANTLR-runtime)