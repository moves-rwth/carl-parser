# Generate antlr grammars
antlr_target(SerializationGrammarLexer
        ${CMAKE_SOURCE_DIR}/src/carl-parser/Serialization.g4
        LEXER PARSER VISITOR
        PACKAGE carlparser
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/resources/generated/carl-parser
)
antlr_target(SmtlibGrammarLexer
        ${CMAKE_SOURCE_DIR}/src/carl-parser/smtlib.g4
        LEXER PARSER VISITOR
        PACKAGE carlparser
        OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/resources/generated/carl-parser
)

set(antlr_generated_include ${CMAKE_BINARY_DIR}/resources/generated/)

add_library(carl-parser SHARED ${CMAKE_SOURCE_DIR}/src/carl-parser/ParseTreeVisitor.cpp ${CMAKE_SOURCE_DIR}/src/carl-parser/Parser.cpp ${ANTLR_SerializationGrammarLexer_CXX_OUTPUTS} ${ANTLR_SmtlibGrammarLexer_CXX_OUTPUTS})
target_include_directories(carl-parser PRIVATE ${ANTLR4_INCLUDE_DIRS} ${antlr_generated_include})
target_link_libraries(carl-parser PUBLIC lib_carl antlr4)

set(PARSER_INCLUDES ${CMAKE_SOURCE_DIR}/src ${ANTLR_generated_include} ${ANTLR4_INCLUDE_DIRS})

target_include_directories( carl-parser PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src>
        "$<BUILD_INTERFACE:${PARSER_INCLUDES}>"
)
