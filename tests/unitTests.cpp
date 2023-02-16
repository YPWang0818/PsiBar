#include "unitTests.h"


namespace UnitTests {


	void runAll(int argc, char* argv[])
	{
		runParserTests();
		
	};

	void runParserTests()
	{
		
		static std::vector<std::string> testSrc = {
			"(+ 1 1)",
			"(+ 23 343); comments",
			"(* 1 22)",	
			"(* 1.2 1e-5 )",
			"((+ 1.7 1))"
		};


		using namespace PsiBar;

		ExprParser testParser;
		Ref<Expr> result = nullptr;

		for (auto src : testSrc) {

			result = testParser.Parse(src);
			PSIBAR_INFO(result->debugPrint());
			PSIBAR_ASSERT(result, "Parsing Error.");
			// We will only test wether the parsing is sucessful and not validating the result. 
			// Since the result will be complicated ASTs and are directly integrated to the symbol table.
			// The correctness of the AST should be tested as a part of integration tests. 

		}

		




	};

};