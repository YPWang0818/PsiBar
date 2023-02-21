#include "main.h"

using namespace PsiBar;

int main(int argc, char* argv[]) {


	if (argc >= 2) {

		PSIBAR_INFO("Execute scripts ...");
		return 0;
	}

	// Enter interactive shell.

	std::string input; int count = 1;
	std::printf("Interative mode: enter :h or :help for more help. \n");

	ExprParser inputParser;
	Ref<Expr> res = nullptr;


	while (true) {

		std::printf("[%d] ", count);

		std::getline(std::cin, input);



		// Parsing interative mode options.
		if (input[0] == ':') {
			if (input == ":h" || input == ":help") printHelpMsg();
			else if (input == ":q") break;


		};

		// Parse and output the parse tree. 
		res = inputParser.Parse(input);
		if (res) PSIBAR_INFO("\n" + res->debugPrint());

		//std::printf("%s\n", input.c_str());

		count++;



	}

	return 0;
};

namespace PsiBar {


	void printHelpMsg() {

		std::printf("\t :h  Print help message. \n");
		std::printf("\t :q  Quit interactive mode. \n");

	};

};