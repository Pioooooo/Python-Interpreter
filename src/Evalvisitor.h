#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3BaseVisitor.h"
#include "Object.h"
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <utility>
#include "BigInt.h"

class EvalVisitor: public Python3BaseVisitor
{
public:
	EvalVisitor();

private:
	typedef std::unordered_map<std::string, Object> OMap;
	typedef std::vector<OMap> MVector;

	std::vector<MVector> variables;

	OMap &global()
	{
		return variables[0][0];
	}

	Object FindVar(const Object &name);

	std::unordered_map<std::string, Object>::iterator GetVar(const Object &name);

	Object &InsertVar(const Object &name, const Object &val);

	Object &AssignVar(const Object &name, const Object &val);

private:
	class Function
	{
	public:
		typedef std::vector<std::pair<std::string, Object>> Parameters;
		Parameters params;

		Python3Parser::SuiteContext *suite;

	public:
		Function() = default;

		Function(Parameters __params, Python3Parser::SuiteContext *__suite):

				params(__params), suite(__suite)
		{
		}
	};

	std::unordered_map<std::string, Function> functable;

private:
	antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override;

	antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;

	antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override;

	antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override;

	antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override;

	antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override;

	antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override;

	antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override;

	antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;

	antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override;

	antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override;

	antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override;

	antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override;

	antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override;

	antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override;

	antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override;

	antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override;

	antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override;

	antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override;

	antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override;

	antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override;

	antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override;

	antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override;

	antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override;

	antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;

	antlrcpp::Any visitAddsub_op(Python3Parser::Addsub_opContext *ctx) override;

	antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override;

	antlrcpp::Any visitMuldiv_op(Python3Parser::Muldiv_opContext *ctx) override;

	antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override;

	antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;

	antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override;

	antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override;

	antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override;

	antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override;

	antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override;
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H