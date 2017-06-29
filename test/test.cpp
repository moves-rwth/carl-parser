//
// Created by Sebastian Junges on 28/06/2017.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <antlr4-runtime.h>
#include <tree/ParseTreeVisitor.h>
#include "SerializationLexer.h"
#include "SerializationParser.h"
#include "SerializationVisitor.h"
#include "ParseTreeVisitor.h"
#include "Parser.h"

using namespace carlparser;
using namespace antlr4;

TEST_CASE("testing the parse") {
    auto res = carlparser::deserialize<carl::MultivariatePolynomial<mpq_class>>("(/ 12 12)");
    DOCTEST_CHECK_EQ(check_type(res), carlparser::ParserReturnType::Rational);
    res = carlparser::deserialize<carl::MultivariatePolynomial<mpq_class>>("x");
    DOCTEST_CHECK_EQ(check_type(res), carlparser::ParserReturnType::Variable);
    res = carlparser::deserialize<carl::MultivariatePolynomial<mpq_class>>("(+ x 1)");
    DOCTEST_CHECK_EQ(check_type(res), carlparser::ParserReturnType::Polynomial);

}