#include "ParseTreeVisitor.h"

#include <carl/core/Monomial.h>
#include <carl/core/VariablePool.h>
#include <carl/core/Term.h>
#include <carl/core/MultivariatePolynomial.h>
#include <carl/formula/Formula.h>

#include <string>

namespace carl {
	typedef mpq_class Rational;

	antlrcpp::Any ParseTreeVisitor::visitStart(SerializationParser::StartContext *ctx) {
		return ctx->form_expr()->accept(this);
	}

	antlrcpp::Any ParseTreeVisitor::visitForm_expr(SerializationParser::Form_exprContext *ctx) {
		if (ctx->formula_nary()) {
			return ctx->formula_nary()->accept(this);
		} else if (ctx->formula_unary()) {
			return ctx->formula_unary()->accept(this);
		} else if (ctx->constraint()) {
			return ctx->constraint()->accept(this);
		} else if (ctx->arith_expr()) {
			return ctx->arith_expr()->accept(this);
		}
		return nullptr;
	}

	antlrcpp::Any ParseTreeVisitor::visitFormula_nary(SerializationParser::Formula_naryContext *ctx) {
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitForm_op_nary(SerializationParser::Form_op_naryContext *ctx) {
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitFormula_unary(SerializationParser::Formula_unaryContext *ctx) {
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitConstraint(SerializationParser::ConstraintContext *ctx) {
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitRelation(SerializationParser::RelationContext *ctx) {
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitArith_expr(SerializationParser::Arith_exprContext *ctx) {
		if (ctx->arith_nary()) {
			return ctx->arith_nary()->accept(this);
		} else if (ctx->atom()) {
			return ctx->atom()->accept(this);
		}
		return nullptr;
	}

	antlrcpp::Any ParseTreeVisitor::visitArith_nary(carl::SerializationParser::Arith_naryContext *ctx) {
		std::cout << ctx->token->getText() << std::endl;
		return ParseTreeVisitor::visitChildren(ctx);
	}

	antlrcpp::Any ParseTreeVisitor::visitAtom(SerializationParser::AtomContext *ctx) {
		if (ctx->number()) {
			return ctx->number()->accept(this);
		} else if (ctx->variable()) {
			return ctx->variable()->accept(this);
		} else if (ctx->TRUE()) {
			return carl::Formula<carl::MultivariatePolynomial<Rational>>(FormulaType::TRUE);
		} else if (ctx->FALSE()) {
			return carl::Formula<carl::MultivariatePolynomial<Rational>>(FormulaType::FALSE);
		}
		return nullptr;
	}

	antlrcpp::Any ParseTreeVisitor::visitNumber(SerializationParser::NumberContext *ctx) {
		Rational tmp;
		if (ctx->NUMERAL()) {
			bool suc = carl::try_parse<Rational>(ctx->NUMERAL()->getText(), tmp);
			if(!suc) {
				throw std::invalid_argument("Cannot translate " + ctx->NUMERAL()->getText() + " into a rational.");
			}
			return tmp;
		} else if (ctx->DECIMAL()) {
			bool suc = carl::try_parse<Rational>(ctx->DECIMAL()->getText(), tmp);
			if(!suc) {
				throw std::invalid_argument("Cannot translate " + ctx->DECIMAL()->getText() + " into a rational.");
			}
			return tmp;
		} else if (ctx->HEXADECIMAL()) {
			long v = std::stol(ctx->HEXADECIMAL()->getText().substr(2), nullptr, 16);
			tmp = v;
		} else if (ctx->BINARY()) {
			long v = std::stol(ctx->BINARY()->getText().substr(2), nullptr, 2);
			tmp = v;
		}
		return nullptr;
	}

	antlrcpp::Any ParseTreeVisitor::visitVariable(SerializationParser::VariableContext *ctx) {
		std::string name = ctx->SYMBOL()->getSymbol()->getText();
		//try and generate new variable from pool
		Variable newVar;
		auto &pool = VariablePool::getInstance();
		Variable res = pool.findVariableWithName(name);
		if (res != Variable::NO_VARIABLE) {
			newVar = res;
		} else {
			newVar = freshRealVariable(name);
		}

		return newVar;
	}
}