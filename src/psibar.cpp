#include "psibar.h"
#include "logger.h"

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
		m_idx = 0;


		std::string_view src_view{ src.c_str(), src.size() };
		parseExpression(src_view);

		return 0;
	};

	std::string_view ExprParser::nextToken(std::string_view src)
	{
		std::string_view t = lookToken(src);
		m_idx += t.size();

		return t;
	};

	std::string_view ExprParser::lookToken(std::string_view src, std::size_t step)
	{
		

		for (m_idx; (isspace(src[m_idx]) || src[m_idx] == ';') && m_idx < src.size(); m_idx++) {
			if (src[m_idx] == ';') while (src[m_idx] != '\n' && m_idx < src.size()) m_idx++;
		};
		
		size_t forward;

		for (forward = m_idx; !isspace(src[forward]) && forward < src.size(); forward++) {};
		
		if (forward == src.size()) { return std::string_view(""); }; // End of file, return an empty token.
		
		return src.substr(m_idx, (forward - m_idx - 1));

	}



	void ExprParser::parseExpression(std::string_view src)
	{

		if (lookToken(src) != "(") {

			return; //error.
		};

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken(src) == "+") {
			nextToken(src); while ( lookToken(src) != ")" ){  parseTerm(src); }; nextToken(src);

			Ref<Expr> node = CreateRef<Expr>(ExprType::EXPR);
			
			createNode(node);
			return;
		}
		else {
			parseTerm(src);
			return;
		};

		
	
	};

	void ExprParser::parseTerm(std::string_view src) {


		if (lookToken(src) != "(") { return; };

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken(src) == "*") {
			nextToken(src); while (nextToken(src) != ")") { parseFactor(src); }; nextToken(src);

			Ref<Expr> node = CreateRef<Expr>(ExprType::TERM);

			createNode(node);
			return;

		}
		else {
			parseFactor(src);
		};


		
	};



	void ExprParser::parseFactor(std::string_view src)
	{
		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken(src) == "(") {

			if (lookToken(src, 2) == ":g") {
				parseGenerator(src);

				Ref<Expr> node = CreateRef<Expr>(ExprType::GEN);
				createNode(node);
				return;


			};

			if (lookToken(src, 2) == "(") {

				if (lookToken(src, 3) == "(") {

					nextToken(src); parseExpression(src);  nextToken(src);
					return;
				};

				if (lookToken(src, 3) == ":d") {
					nextToken(src); while (nextToken(src) != ")") { parseDerFactor(src); };  nextToken(src);

					Ref<Expr> node = CreateRef<Expr>(ExprType::FACTOR);
					parseGenerator(src);

					createNode(node);
					return;
				};

				//error.
			};

			//error

		};

		int64_t natTokenVal = 0;
		if (isNat(lookToken(src), &natTokenVal) ) {

			Ref<Expr> node = CreateRef<Expr>(ExprType::NAT);
			node->nat = natTokenVal;
			createNode(node);
			return;

		};


		double realTokenVal = 0;
		if (isReal(lookToken(src), &realTokenVal)) {

			Ref<Expr> node = CreateRef<Expr>(ExprType::REAL);
			node->real = realTokenVal;
			createNode(node);
			return;
		};


		return;
	};

	void ExprParser::parseGenerator(std::string_view src)
	{
		return;
	};


	void  ExprParser::parseDerFactor(std::string_view src)
	{

		if (lookToken(src) != "(" || lookToken(src, 2) != "d:") { return; };  //Error 
		nextToken(src); nextToken(src);

		
		DerRef der;
		Ref<Expr> derexp, derFactorExp, natexp = nullptr;

		if (isId(lookToken(src))) {

			der = CreateRef<Derivation>(nextToken(src));
			derexp = CreateRef<Expr>(ExprType::DER);
			derexp->der = der;
		}
		else {
			//error.
		};


		int64_t natval = 0;
		
		if (isNat(nextToken(src), &natval) ) {
			natexp = CreateRef<Expr>(ExprType::NAT);
			natexp->nat = natval;
		};
		
		
		derFactorExp = CreateRef<Expr>(ExprType::DERFACTOR);
		derFactorExp->exprs.push_back(derexp);
		if(natexp) derFactorExp->exprs.push_back(natexp);

		push(derFactorExp);

		return;
	};

	void ExprParser::parseTag(std::string_view tag){

		return;
	};


	void ExprParser::createNode(Ref<Expr> node)
	{
		return;
	};


	void ExprParser::push(Ref<Expr> gen)
	{

	}
	void ExprParser::pop()
	{
	}
	;


	bool ExprParser::isNat(std::string_view token, int64_t* value)
	{
		return false;
	};


	bool ExprParser::isReal(std::string_view token, double* value)
	{
		return false;
	}
	bool ExprParser::isId(std::string_view token)
	{
		return false;
	}
	;

};

