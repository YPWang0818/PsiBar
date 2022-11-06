#include "psibar.h"

namespace PsiBar {


	
	int defineNewSymbol(const std::string& name, Ref<Symbol>* symb, Parity parity, bool scalar)
	{

		if (GET_SYMTB()->find(name) != GET_SYMTB()->end()) {
			return W_RESOURCE_DUPICATE;
		};

		Ref<Symbol> syb = CreateRef<Symbol>(name);

		syb->setIsScalar(scalar);
		syb->setPairy(parity);

		if (symb) *symb = syb;
		(*GET_SYMTB())[name] = syb;

		return _OK;
	}

	int defineNewDerivation(const std::string& name, Ref<Derivation>* der)
	{

		if (GET_DERTB()->find(name) != GET_DERTB()->end()) {
			return W_RESOURCE_DUPICATE;
		};

		Ref<Derivation> derivation = CreateRef<Derivation>(name);

		if (der) *der = derivation;
		(*GET_DERTB())[name] = derivation;

		return _OK;
	};


	GlobalContext::GlobalContext()
	{
		
		symbolTable = CreateRef<SymbTb>();
		derivationTable = CreateRef<DerTb>();
	}


	int ExprParser::Parse(const std::string& src, Ref<Expr>& output)
	{
		m_stack.resize(0);
		m_sp = 0; m_bp = 0;

		std::string_view src_view{ src.c_str(), src.size() };

		parseExpression(src_view, 0);
		auto& end = src.end();

		return 0;
	};


	int ExprParser::parseExpression(std::string_view src, std::size_t idx)
	{

		while ( idx < src.size() ) {

			if (isspace(src[idx])) {
				continue;
			}
			else if (src[idx] == '(') {
				m_stack.push_back(nullptr); // Create an nullptr as the start of the stack frame.
				parseExpression(src, ++idx);
			}
			else if (src[idx] == ')') {
				return flushStack();
			}
			else {
				idx += parseTag(src, idx);
			};

			idx++;
		};

		return 0;
	};

	int ExprParser::parseTag(std::string_view src, std::size_t idx)
	{
		std::size_t sidx = idx;
		while ( !isspace(src[idx])  && (idx < src.size())) idx++;
		
		std::string_view tag_view = src.substr(sidx, idx - 1);


		if ( isTagGen(tag_view) ) {
			
			Ref<Expr> g = CreateRef<Expr>(ExprType::GEN);
			g->gen = CreateRef<Generator>(tag_view.data());
			push(g);

			// Strings here.
			return (idx - sidx);
		} else if ( isTagNat(tag_view) ) {

			Ref<Expr> g = CreateRef<Expr>(ExprType::NAT);
			g->nat = std::stoll(tag_view.data());
			push(g);

			// Numbers here.
			return (idx - sidx);
		} else if ( isTagReal(tag_view) ) {

			Ref<Expr> g = CreateRef<Expr>(ExprType::REAL);
			g->real = std::stod(tag_view.data());

			push(g);

			// Float here.
			return (idx - sidx);
		}
		else {
		
			
		};

	
		
		return 0;
	};

	int ExprParser::flushStack()
	{
		

		return 0;
	}
	void ExprParser::push(Ref<Expr> gen)
	{
	}


	bool ExprParser::isTagGen(std::string_view tag)
	{
		return false;
	}
	bool ExprParser::isTagNat(std::string_view tag)
	{
		return false;
	}
	bool ExprParser::isTagReal(std::string_view tag)
	{
		return false;
	}
};

