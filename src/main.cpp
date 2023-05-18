#include "util.h"
#include "logger.h"
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
	CommandParser commandParser;

	Ref<Expr> res = nullptr;
	Ref<InputCommand> cmd = nullptr;

	while (true) {

		std::printf("[%d] ", count);

		std::getline(std::cin, input);


		// Parsing interative mode options.
		if (input[0] == ':') {
			cmd = commandParser.parseInput(input);
			PSIBAR_INFO("\n" + cmd->debugPrint());

			switch (cmd->type) {
			case CommandType::HELP:
				printHelpMsg();
				break;
			case CommandType::LOCAL:
				break;
			case CommandType::SETDER:
			case CommandType::SETGEN:
				parseSetVarInput(cmd);

				break;
			case CommandType::UNDEF:
				PSIBAR_ERR("\n Undefined command.");
				printHelpMsg();
				break;
			};


			continue;

		};

	
		//parseInput and output the parse tree. 
		res = inputParser.parseInput(input);
		if (res) PSIBAR_INFO("\n" + res->debugPrint());

		std::printf("%s\n", input.c_str());

		count++;

	}

	return 0;
};

namespace PsiBar {


	void printHelpMsg() {

		std::printf("\t :h  Print help message. \n");
		std::printf("\t :q  Quit interactive mode. \n");

	}

	void parseSetVarInput(const Ref<InputCommand>& cmd)
	{

		if (cmd->type == CommandType::SETDER) {
			DerivationParser derivationParser;
			Ref<Derivation> der = derivationParser.parseInput(cmd->rest);
			if (der) PSIBAR_INFO("\n" + der->debugPrint());
		};

		if (cmd->type == CommandType::SETGEN) {
			GeneratorParser generatorParser;
			Ref<Generator> gen = generatorParser.parseInput(cmd->rest);
			if (gen) PSIBAR_INFO("\n" + gen->debugPrint());	
		}

	
	};



};


