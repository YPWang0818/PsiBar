#include "parser.h"


namespace PsiBar {



	ExprParser::ExprParser()
	{
	}

	void ExprParser::parseFull()
	{
		parseExpression();
	};



	void ExprParser::parseExpression()
	{

		push(); // Push an empty expression on the stack to signal the start of the stack.

		if (lookToken() != "(") {

			pop();  parseFactor();
			return;
		};

		if (lookToken(2) == "+") {
			nextToken(2); while (lookToken() != ")") {
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
	}

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
					nextToken(); while (lookToken() == "(" && lookToken(2) == ":d") {
						parseDerFactor();
					};
					nextToken();
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

				pop(); parseExpression();
				return;


			}

			onError("Invalid syntax.");

		};

		int64_t natTokenVal = 0;
		if (isNat(lookToken(), &natTokenVal)) {
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
			return;
		};  //Error 

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


	void ExprParser::parseGenProp(Ref<Function> gen)
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

		std::string_view genproptk = nextToken();

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


	CommandParser::CommandParser()
	{
	};

	void CommandParser::parseFull()
	{
		Ref<InputCommand> command = CreateRef<InputCommand>();

		if (lookToken() == ":set") {
			if (lookToken(2) == "der")  command->type = CommandType::SETDER;
			else if (lookToken(2) == "gen")  command->type = CommandType::SETGEN;

			nextToken(2);
		}
		else if (lookToken() == ":help" || lookToken() == ":h") {
			command->type = CommandType::HELP;
			nextToken();
		}
		else if (lookToken() == ":local") {
			command->type = CommandType::LOCAL;
			nextToken();
		}
		else {
			command->type = CommandType::UNDEF;
		};

		command->rest = getSrcView().substr(getPosition());

		push(command);
	}

	#ifdef PSIBAR_DEBUG

	std::string InputCommand::debugPrint()
	{
		std::stringstream ss;

		switch (type) {
		case CommandType::HELP:
			ss << "[HELP] ";
			break;
		case CommandType::LOCAL:
			ss << "[LOCAL] ";
			break;
		case CommandType::SETDER:
			ss << "[SET DER] ";
			break;
		case CommandType::SETGEN:
			ss << "[SET GEN] ";
			break;
		case CommandType::UNDEF:
			ss << "[UNDEF] ";
			break;
		};

		ss << "\n";
		ss << "rest: " << std::string{rest};
		ss << "\n";

		return ss.str();
	}


	#endif 


};