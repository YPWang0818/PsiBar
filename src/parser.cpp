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
			Ref<Generator> gen = CreateRef<Generator>(gentoken);
			Ref<Expr> genexpr = CreateRef<Expr>(gen);
			push(genexpr);

			return;
		};


		//Otherwise the generator is a function.

		Ref<Generator> gen = CreateRef<Generator>(gentoken);
		while (lookToken() != ")") {
			parseGenProp(gen);
		} nextToken();
		Ref<Expr> genexp = CreateRef<Expr>(gen);
		push(genexp);

		return;
	};


	void ExprParser::parseGenProp(Ref<Generator> gen)
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

		if (!(gen->haveArg(lookToken()))) {


			onError("Generator properties dosn't exists.");
			return; //Error.
		};


		parseExpression();
		// Get the experssion just parsed and set it as  the value correspond to the argument.
		gen->setArg(std::string{ nextToken() }, pop());

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



	void DerivationParser::parseFull()
	{

		if (!isId(lookToken())) {

			onError("Invalid Derivation name.");
			return; //Error.
		};

		std::string_view derName = nextToken();

		Ref<Derivation> derivation = CreateRef<Derivation>(derName);

		if (lookToken() == ":suffix") {
			nextToken();
			derivation->setHasSuffix(true);
		};

		push(derivation);


	};


	void GeneratorParser::parseFull()
	{

		if (!isId(lookToken())) {

			onError("Invalid Generator name.");
			return; //Error.
		};

		std::string_view genName = nextToken();
		Ref<Generator> generator = CreateRef<Generator>(genName);
		
		
		while (lookToken() != "") {
			if (lookToken() == "(") {

				if (lookToken(2) == ":prop") {
					parseGenProp(generator);
				}
				else if (lookToken(2) == ":parity") {
					parsePType(generator);
				}
				else {
					onError("Invaild generator.");
				};

			} else if (lookToken() == ":scalar") {
				generator->setIsScalar(true);
				nextToken();
			}
			else {
				onError("Invaild generator.");
			};
		};

		push(generator);


	}

	void GeneratorParser::parseGenProp(Ref<Generator> gen)
	{
		nextToken(2);

		while (lookToken() != ")") {
			if (!isId(lookToken())) {

				onError("Invalid Generator Properties name.");
				return;
			};

			std::string_view genprop = nextToken();
			gen->setArg(std::string{ genprop });
		};

		nextToken();

	}

	void GeneratorParser::parsePType(Ref<Generator> gen)
	{
		nextToken(2);


		boost::regex evenReg { "([Ee]ven|EVEN|1)" };
		boost::regex oddReg{ "([Oo]dd|ODD|-1)" };
		boost::regex noneReg{ "([Nn]one|NONE|0)" };


		std::string_view type = lookToken();
		
		if (boost::regex_match(std::string{ type }, evenReg)) {
			gen->setPairy(Parity::EVEN);
		}
		else if (boost::regex_match(std::string{ type }, oddReg)) {
			gen->setPairy(Parity::ODD);
		}
		else if (boost::regex_match(std::string{ type }, noneReg)) {
			gen->setPairy(Parity::NONE);
		}
		else {
			onError("Unrecognized parity type.");
		};

		nextToken(2);

	}

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
		ss << "rest: " << std::string{ rest };
		ss << "\n";

		return ss.str();
	}


	#endif 




};