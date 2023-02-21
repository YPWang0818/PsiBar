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


	Ref<Expr>  ExprParser::Parse(const std::string& src)
	{

		try {

			Reset();

			m_srcView = std::string_view{ src.c_str(), src.size() };

			for (int i = 0; i < m_tokenBufferSz; i++) {
				m_tokenBuffer.push_back(getToken());
			};

			parseExpression();
			return pop();
		}
		catch (ParseException e) {
		
			PSIBAR_ERR(e.what());
			return nullptr;
		}

	};

	void ExprParser::Reset()
	{
		m_srcView = std::string_view{ nullptr, 0 };
		m_stack.clear();
		m_tokenBuffer.clear();
		m_idx = 0;
		m_errorFlag = false;
	};

	std::string_view ExprParser::getToken()
	{
		// Iterating over the spaces and comments.
		for (; m_idx < m_srcView.size() && (isspace(m_srcView[m_idx]) || m_srcView[m_idx] == ';'); m_idx++) {
			if (m_srcView[m_idx] == ';') while (m_idx < m_srcView.size() && m_srcView[m_idx] != '\n') {
				m_idx++;
			};
		};


		// End of file, return an empty token pointing to the end of the source.
		if (m_idx >= m_srcView.size()) { return m_srcView.substr(m_srcView.size(), 0); };

		std::string_view token;
		

		// The token is the special character '(' and ')'. This is to prevent code such as "(+ " or "(variable)" to be
		// identified as one token.
		if (m_srcView[m_idx] == '(' || m_srcView[m_idx] == ')' ) {

			token = m_srcView.substr(m_idx, 1); m_idx++;
			return token;
	
		};


		// The iterate through the token, which is deliminated by an isspace() character.
		size_t forward = 0;
		for (;
			( (m_idx + forward) < m_srcView.size() )	&&
			!isspace(m_srcView[m_idx + forward])		&&
			m_srcView[m_idx + forward] != ';'			&&
			m_srcView[m_idx + forward] != '('			&& 
			m_srcView[m_idx + forward] != ')'	// To avoid code such as +) registored as one token.
			; forward++) { };

		token = m_srcView.substr(m_idx, forward);
		m_idx += forward;
		return token;

	}

	std::string_view ExprParser::nextToken(std::size_t count)
	{
		m_tokenBuffer.push_back(getToken());
		std::string_view& res = m_tokenBuffer.front();
		m_tokenBuffer.pop_front();

		for (int c = 1; c < count; c++) {

			m_tokenBuffer.push_back(getToken());
			res  = m_tokenBuffer.front();
			m_tokenBuffer.pop_front();
		};

		return res;
	};

	std::string_view ExprParser::lookToken(std::size_t step)
	{
		// For effeciency, we don't check the bound here. ( Bad idea?)

		
		return m_tokenBuffer[(step - 1)];
	}



	void ExprParser::parseExpression()
	{

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken() != "(") {

			pop();  parseFactor();
			return;
		};

		if (lookToken(2) == "+") {
			nextToken(2); while ( lookToken() != ")" ){  
				parseTerm(); 
			}; nextToken();

			Ref<Expr> node = CreateRef<Expr>(ExprType::EXPR);
			
			createNode(node); 
			return;
		}
		else {
			pop();  parseTerm();
			return;
		};

		
	
	};

	void ExprParser::parseTerm() {

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken() != "(") { 

			pop();  parseFactor();
			return; 
		};

	

		if (lookToken(2) == "*") {
			nextToken(2); while (lookToken() != ")") { 
				parseFactor(); 
			}; nextToken();

			Ref<Expr> node = CreateRef<Expr>(ExprType::TERM);

			createNode(node); 
			return;

		}
		else {
			pop();  parseFactor();
		};


		
	};



	void ExprParser::parseFactor()
	{
		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken() == "(") {

			if (lookToken(2) == ":g") {
				parseGenerator();

				Ref<Expr> node = pop();
				pop(); push(node);
				return;


			};

			if (lookToken(2) == "(") {

				if (lookToken(3) == ":d") {
					nextToken(); while ( lookToken() == "(" && lookToken(2) == ":d" ) { 
						parseDerFactor(); 
					}; 
					parseGenerator();
					Ref<Expr> node = CreateRef<Expr>(ExprType::FACTOR);
					createNode(node); 

					nextToken();
					return;
				};

				pop();
				nextToken(); parseExpression();  nextToken();
				return;
			};

			if (lookToken(2) == "*" || lookToken(2) == "+") {

				pop();parseExpression(); 
				return;
			
			
			}

			onError("Invalid syntax.");

		};

		int64_t natTokenVal = 0;
		if (isNat(lookToken(), &natTokenVal) ) {
			nextToken();
			Ref<Expr> node = CreateRef<Expr>(ExprType::NAT);
			node->nat = natTokenVal;
			createNode(node); 	
			return;

		};


		double realTokenVal = 0;
		if (isReal(lookToken(), &realTokenVal)) {
			nextToken();
			Ref<Expr> node = CreateRef<Expr>(ExprType::REAL);
			node->real = realTokenVal;
			createNode(node);  
			return;
		};

		onError("Invalid syntax.");
		return;
	};

	void ExprParser::parseGenerator()
	{

		if (lookToken() != "(" || lookToken(2) != ":g") {
			onError("Invalid generator syntax.");
			return; };  //Error 

		// push(); // No need to do that here, since there is no extra states need to be saved on the stack.

		nextToken(); nextToken();


		// Test to see if next token is a genarator. Since we don't know the generator type yet. Defer the creation 
		// of the instnace later. 

		if (!isId(lookToken())) {

			onError("Invalid identifier.");
			return; //Error.
		};


		std::string_view gentoken = nextToken();


		// If the generator is a symbol.
		if (lookToken() == ")") {
			nextToken();
			Ref<Symbol> gen = CreateRef<Symbol>(gentoken);
			Ref<Expr> genexpr = CreateRef<Expr>(gen);
			push(genexpr);

			return;
		};


		//Otherwise the generator is a function.

		Ref<Function> gen = CreateRef<Function>(gentoken);
		while (lookToken() != ")") {
			parseGenProp(gen);
		} nextToken();
		Ref<Expr> genexp = CreateRef<Expr>(gen);
		push(genexp);

		return;
	};


	void ExprParser::parseGenProp( Ref<Function> gen)
	{
		if (lookToken() != "(") {

			onError("No opening bracket.");

			return; // Error.
		}  
		
		nextToken();

		if (!isId(lookToken())) {

			onError("Invalid identifier.");
			return; //Error.
		};

		if (!(gen->haveTag(lookToken()))) {

			onError("Generator properties dosn't exists.");
			return; //Error.
		};


		parseExpression();
		// Get the experssion just parsed and set it as  the value correspond to the argument.
		gen->args()[std::string{ nextToken() }] = pop();

		nextToken();


	};


	void  ExprParser::parseDerFactor()
	{

		if (lookToken() != "(" || lookToken(2) != ":d") { 
			
			//onError("Invalid syntax for a derivation.");
			return; 
		};  

		// push(); // No need to do that here, since there is no extra states need to be saved on the stack.

		nextToken(2);


		DerRef der;
		Ref<Expr> derFactorExp;

		// Test to see if next token is a derivation. If true, construct a instnance of it.
		if (isId(lookToken())) {
			der = CreateRef<Derivation>(nextToken());
		}
		else {
			onError("Invalid identifier for derivation.");
			//error.
		};

		int64_t powval = 1;

		// See if the derivation expression has a power specifier. 
		if (isNat(nextToken(), &powval));


		derFactorExp = CreateRef<Expr>(der, powval);

		// The node is created manually here. 
		push(derFactorExp);

		return;
	};

	


	void ExprParser::createNode(Ref<Expr> node)
	{
		if (m_stack.empty()) return;
		
		while (m_stack.back() != nullptr) {
			
			node->exprs.push_back(m_stack.back());
			m_stack.pop_back();
		};
		pop(); push(node);
		return;
	};


	void ExprParser::push(Ref<Expr> gen)
	{
		m_stack.push_back(gen);

	}

	Ref<Expr> ExprParser::pop()
	{
		if(m_stack.empty()) return nullptr;
		
		Ref<Expr> res = m_stack.back();
		m_stack.pop_back();

		return res;
	};


	bool ExprParser::isNat(std::string_view token, int64_t* value)
	{


		// Should use regular expression in future when dealing with arbitrary percision integers. 

		int64_t result, i;
		std::sscanf(std::string{ token }.c_str(), "%lli%lln", &result, &i);

		// Only match if scanf consumes the whole token.
		if (i != token.size()) { return false; };

		*value = result;
		return true;
	};


	bool ExprParser::isReal(std::string_view token, double* value)
	{

		int64_t i; double result;

		std::sscanf(std::string{ token }.c_str(), "%lg%lln", &result, &i);

	
		if (i != token.size()) { return false; };

		*value = result;
		return true;

	};


	bool ExprParser::isId(std::string_view token)
	{
		boost::regex idreg{ "[\\w][\\w\\-]*\\:?" };
		//boost::regex idreg{ "[a-zA-Z]*" };
		
		return boost::regex_match(std::string{ token }, idreg);
	};


	void ExprParser::onError(std::string msg, ErrCallback callback)
	{
		if (callback) { callback(this); return; };

		throw ParseException(msg);

	};

};

