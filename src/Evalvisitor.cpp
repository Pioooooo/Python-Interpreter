#include "Evalvisitor.h"

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx)
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx)
{
	functable[ctx->NAME()->toString()] = Function(visit(ctx->parameters()).as<Function::Parameters>(), ctx->suite());
	return Object();
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx)
{
	auto tmp = ctx->typedargslist();
	if(tmp)
	{
		return visit(tmp);
	}
	return Function::Parameters();
}

antlrcpp::Any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx)
{
	Function::Parameters ret;
	auto tfpdefs = ctx->tfpdef();
	auto tests = ctx->test();
	for(unsigned i = 0; i < tfpdefs.size() - tests.size(); i++)
	{
		ret.push_back({tfpdefs[i]->NAME()->toString(), Object()});
	}
	for(unsigned i = 0; i < tests.size(); i++)
	{
		ret.push_back({tfpdefs[tfpdefs.size() - tests.size() + i]->NAME()->toString(), visit(tests[i]).as<Object>()});
	}
	return ret;
}

antlrcpp::Any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx)//Useless
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx)
{
	return visit(ctx->children[0]);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx)
{
	return visit(ctx->small_stmt());
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx)
{
	if(ctx->flow_stmt())
	{
		return visit(ctx->flow_stmt());
	}
	visit(ctx->expr_stmt());
	return Object();
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx)
{
	auto testlists = ctx->testlist();
	if(!ctx->augassign())
	{
		auto val = visit(testlists.back()).as<Object>().as<List>();
		for(int i = 0; i < testlists.size() - 1; i++)
		{
			for(unsigned j = 0; j < val.size(); j++)
			{
				AssignVar(testlists[i]->test()[j]->or_test()->and_test()[0]->not_test()[0]->comparison()->arith_expr()[0]->term()[0]->factor()[0]->atom_expr()->atom()->NAME()->toString(), val[j]);
			}
		}
		return val;
	}
	Object name(testlists[0]->test()[0]->or_test()->and_test()[0]->not_test()[0]->comparison()->arith_expr()[0]->term()[0]->factor()[0]->atom_expr()->atom()->NAME()->toString());
	if(!FindVar(name).toBool())
	{
		std::cerr << "NameError: name '" << name.toString() << "' is not defined" << std::endl;
	}
	if(ctx->augassign()->ADD_ASSIGN())
	{
		return GetVar(name)->second += visit(testlists[1]).as<Object>().as<List>()[0];
	}
	if(ctx->augassign()->SUB_ASSIGN())
	{
		return GetVar(name)->second -= visit(testlists[1]).as<Object>().as<List>()[0];
	}
	if(ctx->augassign()->MULT_ASSIGN())
	{
		return GetVar(name)->second *= visit(testlists[1]).as<Object>().as<List>()[0];
	}
	if(ctx->augassign()->DIV_ASSIGN())
	{
		return GetVar(name)->second /= visit(testlists[1]).as<Object>().as<List>()[0];
	}
	if(ctx->augassign()->MOD_ASSIGN())
	{
		return GetVar(name)->second %= visit(testlists[1]).as<List>()[0];
	}
	if(ctx->augassign()->IDIV_ASSIGN())
	{
		return GetVar(name)->second = GetVar(name)->second.idiv(visit(testlists[1]).as<List>()[0]);
	}
}

antlrcpp::Any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx)//Useless
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx)
{
	return visit(ctx->children[0]);
}

antlrcpp::Any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx)
{
	return Object(false);
}

antlrcpp::Any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx)
{
	return Object(true);
}

antlrcpp::Any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx)
{
	if(ctx->testlist())
	{
		return visit(ctx->testlist());
	}
	return 0;
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx)
{
	return visit(ctx->children[0]);
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx)
{
	auto tests = ctx->test();
	auto suites = ctx->suite();
	for(unsigned i = 0; i < tests.size(); i++)
	{
		if(visit(tests[i]).as<Object>().toBool())
		{
			return visit(suites[i]);
		}
	}
	if(suites.size() > tests.size())
	{
		return visit(suites.back());
	}
	return Object();
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx)
{
	while(visit(ctx->test()).as<Object>().toBool())
	{
		Object ret = visit(ctx->suite()).as<Object>();
		if(!ret.is<None>())
		{
			if(!ret.is<bool>())
			{
				return ret;
			}
			if(ret.toBool())
			{
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return Object();
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx)
{
	variables.back().push_back(OMap());
	if(ctx->simple_stmt())
	{
		Object ret = visit(ctx->simple_stmt()).as<Object>();
		if(!ret.is<None>())
		{
			variables.back().pop_back();
			return ret;
		}
	}
	else
	{
		auto stmts = ctx->stmt();
		for(auto &stmt:stmts)
		{
			auto ret = visit(stmt);
			if(!ret.is<Object>())
			{
				return Object();
			}
			auto val = ret.as<Object>();
			if(!val.is<None>())
			{
				variables.back().pop_back();
				return val;
			}
		}
	}
	variables.back().pop_back();
	return Object();
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx)
{
	return visit(ctx->or_test());
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx)
{
	if(ctx->and_test().size() == 1)
	{
		return visit(ctx->and_test()[0]);
	}
	auto and_tests = ctx->and_test();
	for(auto &and_test : and_tests)
	{
		if(visit(and_test).as<Object>().toBool())
			return Object(true);
	}
	return Object(false);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx)
{
	if(ctx->not_test().size() == 1)
	{
		return visit(ctx->not_test()[0]);
	}
	auto not_tests = ctx->not_test();
	for(auto &not_test : not_tests)
	{
		if(!visit(not_test).as<Object>().toBool())
		{
			return Object(false);
		}
	}
	return Object(true);
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx)
{
	if(ctx->comparison())
	{
		return visit(ctx->comparison());
	}
	return !visit(ctx->not_test()).as<Object>();
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx)
{
	auto arith_exprs = ctx->arith_expr();
	auto comp_ops = ctx->comp_op();
	if(comp_ops.size() == 0)
	{
		return visit(arith_exprs[0]).as<Object>();
	}
	Object result(true), former = visit(arith_exprs[0]).as<Object>(), cur;
	for(unsigned i = 0; i < comp_ops.size(); i++)
	{
		if(comp_ops[i]->LESS_THAN())
		{
			result &= former < (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
		if(comp_ops[i]->GREATER_THAN())
		{
			result &= former > (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
		if(comp_ops[i]->LT_EQ())
		{
			result &= former <= (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
		if(comp_ops[i]->GT_EQ())
		{
			result &= former >= (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
		if(comp_ops[i]->EQUALS())
		{
			result &= former == (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
		if(comp_ops[i]->NOT_EQ_2())
		{
			result &= former != (cur = visit(arith_exprs[i + 1]).as<Object>());
			former = cur;
			continue;
		}
	}
	return result;
}

antlrcpp::Any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx)//Useless
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx)
{
	auto terms = ctx->term();
	auto addsub_ops = ctx->addsub_op();
	Object result = visit(terms[0]).as<Object>();
	for(unsigned i = 0; i < addsub_ops.size(); i++)
	{
		if(addsub_ops[i]->ADD())
		{
			result += visit(terms[i + 1]).as<Object>();
			continue;
		}
		if(addsub_ops[i]->MINUS())
		{
			result -= visit(terms[i + 1]).as<Object>();
			continue;
		}
	}
	return result;
}

antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx)
{
	auto factors = ctx->factor();
	auto muldiv_ops = ctx->muldiv_op();
	Object result = visit(factors[0]).as<Object>();
	for(unsigned i = 0; i < muldiv_ops.size(); i++)
	{
		if(muldiv_ops[i]->STAR())
		{
			result *= visit(factors[i + 1]).as<Object>();
			continue;
		}
		if(muldiv_ops[i]->DIV())
		{
			result /= visit(factors[i + 1]).as<Object>();
			continue;
		}
		if(muldiv_ops[i]->IDIV())
		{
			result = result.idiv(visit(factors[i + 1]).as<Object>());
			continue;
		}
		if(muldiv_ops[i]->MOD())
		{
			result %= visit(factors[i + 1]).as<Object>();
			continue;
		}
	}
	return result;
}

antlrcpp::Any EvalVisitor::visitAddsub_op(Python3Parser::Addsub_opContext *ctx)//Useless
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitMuldiv_op(Python3Parser::Muldiv_opContext *ctx)//Useless
{
	return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx)
{
	if(ctx->atom_expr())
	{
		return visit(ctx->atom_expr());
	}
	if(ctx->addsub_op()->ADD())
	{
		return visit(ctx->factor());
	}
	if(ctx->addsub_op()->MINUS())
	{
		auto tmp = -visit(ctx->factor()).as<Object>();
		return tmp;
	}
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx)//TODO: Lots of things here
{
	if(!ctx->trailer())
	{
		return visit(ctx->atom());
	}
	std::string name = ctx->atom()->NAME()->toString();
	if(name == "print")
	{
		List trailers = visit(ctx->trailer()).as<Object>().as<List>();
		for(unsigned i = 0; i < trailers.size() - 1; i++)
		{
			std::cout << trailers[i].toString() << ' ';
		}
		std::cout << trailers[trailers.size() - 1].toString() << '\n';
		return Object();
	}
	if(name == "int")
	{
		return Object(visit(ctx->trailer()).as<List>()[0].toInt());
	}
	if(name == "float")
	{
		return Object(visit(ctx->trailer()).as<List>()[0].toFloat());
	}
	if(name == "bool")
	{
		return Object(visit(ctx->trailer()).as<List>()[0].toBool());
	}
	if(name == "str")
	{
		return Object(visit(ctx->trailer()).as<List>()[0].toString());
	}
	auto tmp = functable.find(name);
	if(tmp != functable.end())
	{
		variables.push_back(MVector());
		variables.back().push_back(OMap());
		auto function = tmp->second;
		if(ctx->trailer()->arglist())
		{
			auto arguments = ctx->trailer()->arglist()->argument();
			for(int i = 0; i < arguments.size(); i++)
			{
				if(arguments[i]->NAME())
				{
					AssignVar(Object(arguments[i]->NAME()->toString()), visit(arguments[i]->test()));
				}
				else
				{
					AssignVar(function.params[i].first, visit(arguments[i]->test()));
				}
			}
		}
		auto ret = visit(function.suite);
		variables.pop_back();
		return ret;
	}
	else
	{
		std::cerr << "Error: Name " << name << " is not defined." << std::endl;
	}
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx)//Useless
{
	return visit(ctx->arglist());
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx)
{
	if(ctx->NAME())
	{
		std::string name = ctx->NAME()->toString();
		Object Name(name);
		if(FindVar(Name).toBool())
		{
			return GetVar(Name)->second;
		}
		return Name;
	}
	if(ctx->NUMBER())
	{
		std::string number = ctx->NUMBER()->toString();
		if(number.find('.') == std::string::npos)
		{
			return Object(BigInt(number));
		}
		return Object(std::stod(number));
	}
	if(ctx->STRING(0))
	{
		std::string ret;
		auto strings = ctx->STRING();
		for(auto &string : strings)
		{
			std::string tmp = string->toString();
			tmp.erase(tmp.begin());
			tmp.pop_back();
			ret += tmp;
		}
		return Object(ret);
	}
	if(ctx->NONE())
	{
		return Object();
	}
	if(ctx->TRUE())
	{
		return Object(true);
	}
	if(ctx->FALSE())
	{
		return Object(false);
	}
	if(ctx->test())
	{
		return visit(ctx->test());
	}
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx)
{
	auto tests = ctx->test();
	List ret;
	for(auto &test : tests)
	{
		ret.push_back(visit(test).as<Object>());
	}
	Object tmp(ret);
	return tmp;
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx)//TODO: Lots of things here
{
	auto arguments = ctx->argument();
	List ret;
	for(auto &argument : arguments)
	{
		ret.push_back(visit(argument).as<Object>());
	}
	return Object(ret);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx)
{
	return visit(ctx->test());
}

Object EvalVisitor::FindVar(const Object &name)
{
	for(auto it = variables.back().rbegin(); it != variables.back().rend(); it++)
	{
		auto pos = it->find(name.toString());
		if(pos != it->end())
		{
			return Object(true);
		}
	}
	return Object(global().find(name.toString()) != global().end());
}

std::unordered_map<std::string, Object>::iterator EvalVisitor::GetVar(const Object &name)
{
	for(auto it = variables.back().rbegin(); it != variables.back().rend(); it++)
	{
		auto pos = it->find(name.toString());
		if(pos != it->end())
		{
			return pos;
		}
	}
	auto pos = global().find(name.toString());
	if(pos != global().end())
	{
		return pos;
	}
}

Object &EvalVisitor::InsertVar(const Object &name, const Object &val)
{
	auto &tmp = variables.back().back();
	tmp.insert(std::make_pair(name.toString(), val));
	return variables.back().back().find(name.toString())->second;
}

Object &EvalVisitor::AssignVar(const Object &name, const Object &val)
{
	if(FindVar(name).as<bool>())
	{
		return GetVar(name)->second = val;
	}
	else
	{
		return InsertVar(name, val);
	}
}

EvalVisitor::EvalVisitor(): Python3BaseVisitor()
{
	MVector tmp;
	tmp.reserve(100);
	tmp.push_back(OMap());
	variables.push_back(tmp);
}
