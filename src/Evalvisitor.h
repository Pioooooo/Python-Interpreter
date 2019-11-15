#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3BaseVisitor.h"

class EvalVisitor : public Python3BaseVisitor
{
	antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override
	{
		return visitChildren(ctx);
	}

	antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override
	{
		return visitChildren(ctx);
	}
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H
