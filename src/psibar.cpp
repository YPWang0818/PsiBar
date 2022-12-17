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
		m_tokenBuffer.resize(0);
		m_idx = 0;

		std::string_view src_view{ src.c_str(), src.size() };

		for (int i = 0; i < m_tokenBufferSz; i++) {
			m_tokenBuffer.push_back(getToken(src_view));
		};


		parseExpression(src_view);

		return 0;
	};

	std::string_view ExprParser::getToken(std::string_view src)
	{
		// End of file, return an empty token.
		if (m_idx == src.size()) { return std::string_view(""); }; 


		// Iterating over the spaces and comments.
		for (m_idx; ( isspace(src[m_idx]) || src[m_idx] == ';' ) && m_idx < src.size(); m_idx++) {
			if (src[m_idx] == ';') while (src[m_idx] != '\n' && m_idx < src.size()) m_idx++;
		};

		

		// The token is the special character '(' and ')'. This is to prevent code such as "(+ " or "(variable)" to be
		// identified as one token.
		if (src[m_idx] == '(' || src[m_idx] == ')' ) {
			return src.substr(m_idx, 1); 
	
		};


		// The iterate through the token, which is deliminated by an isspace() character.
		size_t forward = 0;
		for (; !isspace(src[m_idx + forward]) && (m_idx + forward) < src.size(); forward++) {};
		return src.substr(m_idx, forward);

	}

	std::string_view ExprParser::nextToken(std::string_view src)
	{
		m_tokenBuffer.push_back(getToken(src));
		return m_tokenBuffer.pop_front();
	};

	std::string_view ExprParser::lookToken(std::string_view src, std::size_t step)
	{
		// For effeciency, we don't check the bound here. ( Bad idea?)
		return m_tokenBuffer[(step - 1)];
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
			
			createNode(node); pop();
			return;
		}
		else {
			parseTerm(src); pop();
			return;
		};

		
	
	};

	void ExprParser::parseTerm(std::string_view src) {


		if (lookToken(src) != "(") { return; };

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken(src) == "*") {
			nextToken(src); while (nextToken(src) != ")") { parseFactor(src); }; nextToken(src);

			Ref<Expr> node = CreateRef<Expr>(ExprType::TERM);

			createNode(node); pop();
			return;

		}
		else {
			parseFactor(src); pop();
		};


		
	};



	void ExprParser::parseFactor(std::string_view src)
	{
		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken(src) == "(") {

			if (lookToken(src, 2) == ":g") {
				parseGenerator(src);

				Ref<Expr> node = CreateRef<Expr>(ExprType::GEN);
				createNode(node); pop();
				return;


			};

			if (lookToken(src, 2) == "(") {

				if (lookToken(src, 3) == "(") {

					nextToken(src); parseExpression(src);  nextToken(src);
					pop();
					return;
				};

				if (lookToken(src, 3) == ":d") {
					nextToken(src); while (nextToken(src) != "(") { parseDerFactor(src); };  nextToken(src);
					parseGenerator(src);

					Ref<Expr> node = CreateRef<Expr>(ExprType::FACTOR);
					createNode(node); 	pop();
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
			createNode(node); 	pop();
			return;

		};


		double realTokenVal = 0;
		if (isReal(lookToken(src), &realTokenVal)) {

			Ref<Expr> node = CreateRef<Expr>(ExprType::REAL);
			node->real = realTokenVal;
			createNode(node);  pop();
			return;
		};


		return;
	};

	void ExprParser::parseGenerator(std::string_view src)
	{

		if (lookToken(src) != "(" || lookToken(src, 2) != "g:") { return; };  //Error 

		// push(); // No need to do that here, since there is no extra states need to be saved on the stack.

		nextToken(src); nextToken(src);


		// Test to see if next token is a genarator. Since we don't know the generator type yet. Defer the creation 
		// of the instnace later. 

		if (!isId(lookToken(src))) { return; }; // Error.
		std::string_view gentoken = nextToken(src);


		// If the generator is a symbol.
		if (lookToken(src) == ")") {
			Ref<Symbol> gen = CreateRef<Symbol>(gentoken);
			Ref<Expr> genexpr = CreateRef<Expr>(gen);
			push(genexpr);

			return;
		};


		//Otherwise the generator is a function.

		Ref<Function> gen = CreateRef<Function>(gentoken);
		while (lookToken(src) != ")") {
			parseGenProp(src, gen);
		}
	
		Ref<Expr> genexp = CreateRef<Expr>(gen);
		push(genexp);

		return;
	};


	void ExprParser::parseGenProp(std::string_view src, Ref<Function> gen)
	{
		if (lookToken(src) != "(") return; //Error.
		nextToken(src);

		if (!isId(lookToken(src))) {
			return; //Error.
		};

		if (!(gen->haveTag(lookToken(src)))) {
			return; //Error.
		};

		parseExpression(src);
		// Get the experssion just parsed and set it as  the value correspond to the argument.
		gen->args()[std::string{ nextToken(src) }] = pop();

		nextToken(src);


	};


	void  ExprParser::parseDerFactor(std::string_view src)
	{

		if (lookToken(src) != "(" || lookToken(src, 2) != "d:") { return; };  //Error 

		// push(); // No need to do that here, since there is no extra states need to be saved on the stack.

		nextToken(src); nextToken(src);


		DerRef der;
		Ref<Expr> derFactorExp;

		// Test to see if next token is a derivation. If true, construct a instnance of it.
		if (isId(lookToken(src))) {
			der = CreateRef<Derivation>(nextToken(src));
		}
		else {
			//error.
		};

		int64_t powval = 1;

		// See if the derivation expression has a power specifier. 
		if (isNat(nextToken(src), &powval));


		derFactorExp = CreateRef<Expr>(der, powval);

		// The node is created manually here. 
		push(derFactorExp);

		return;
	};

	


	void ExprParser::createNode(Ref<Expr> node)
	{
		return;
	};


	void ExprParser::push(Ref<Expr> gen)
	{

	}

	Ref<Expr> ExprParser::pop()
	{
		return nullptr;
	};


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

