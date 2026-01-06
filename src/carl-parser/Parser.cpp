//
// Created by Sebastian Junges on 29/06/2017.
//
#include "Parser.h"

// Generated files:
#include "carl-parser/SerializationLexer.h"
#include "carl-parser/SerializationParser.h"
#include "carl-parser/SerializationVisitor.h"
#include "ParseTreeVisitor.h"

#include <antlr4-runtime.h>


using namespace antlr4;

namespace carlparser {
    class ErrorListener : public antlr4::ANTLRErrorListener {
    public:
        void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
                         size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override {
            throw std::runtime_error(std::string("Parser error: ") + msg);
        }

        void reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
                             size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts,
                             antlr4::atn::ATNConfigSet *configs) override {
            throw std::runtime_error("Parser error");
        }

        void reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                         const antlrcpp::BitSet &conflictingAlts,
                                         antlr4::atn::ATNConfigSet *configs) override {
            throw std::runtime_error("Parser error");
        }

        void reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                      size_t prediction, atn::ATNConfigSet *configs) override {
            throw std::runtime_error("Parser error");
        }
    };

    template<typename Pol>
    parser_types<Pol> deserialize(std::string input) {
        ANTLRInputStream input_stream(input);
        SerializationLexer lexer(&input_stream);
        CommonTokenStream tokens(&lexer);
        SerializationParser parser(&tokens);
        ParseTreeVisitor <Pol> visitor;

        auto errListener = ErrorListener();
        lexer.removeErrorListeners();
        lexer.addErrorListener(&errListener);
        parser.removeErrorListeners();
        parser.addErrorListener(&errListener);

        tree::ParseTree *tree = parser.start();
        antlrcpp::Any resultData = tree->accept(&visitor);

        if (resultData.type() == typeid(carl::Formula<Pol>)) {
            return std::any_cast<carl::Formula<Pol>>(resultData);
        } else if (resultData.type() == typeid(carl::Constraint<Pol>)) {
            return std::any_cast<carl::Constraint<Pol>>(resultData);
        } else if (resultData.type() == typeid(carl::RationalFunction<Pol>)) {
            return std::any_cast<carl::RationalFunction<Pol>>(resultData);
        } else if (resultData.type() == typeid(Pol)) {
            return std::any_cast<Pol>(resultData);
        } else if (resultData.type() == typeid(carl::Term<typename Pol::CoeffType>)) {
            return std::any_cast<carl::Term<typename Pol::CoeffType>>(resultData);
        } else if (resultData.type() == typeid(carl::Monomial::Arg)) {
            return std::any_cast<carl::Monomial::Arg>(resultData);
        } else if (resultData.type() == typeid(typename Pol::CoeffType)) {
            return std::any_cast<typename Pol::CoeffType>(resultData);
        } else if (resultData.type() == typeid(carl::Variable)) {
            return std::any_cast<carl::Variable>(resultData);
        } else {
            throw std::runtime_error("Unexpected parser result");
        }
    }

    template
    parser_types<carl::MultivariatePolynomial<mpq_class>> deserialize(std::string);
#ifdef USE_CLN_NUMBERS
    template
    parser_types<carl::MultivariatePolynomial<cln::cl_RA>> deserialize(std::string);
#endif
#ifdef USE_MPFR_FLOAT
    template
	class ParseTreeVisitor<MultivariatePolynomial<FLOAT_T<mpfr_t>>>;
#endif
#ifdef USE_Z3_NUMBERS
    template
	class ParseTreeVisitor<MultivariatePolynomial<rational>>;
#endif

}