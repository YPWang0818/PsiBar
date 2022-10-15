#include <iostream>

#include "src/expr.h"
#include "src/phibar.h"

void main(int argc, char* argv[]) {

	using namespace PsiBar;

	Ref<Symbol> s1;
	Ref<Derivation> d1;

	if (defineNewSymbol("alpha", &s1, Parity::EVEN, true)) {
		std::cout << "Cannot create symbol " << std::endl;
	} else {
		std::cout << s1->debugPrint() << std::endl;
	};

	if (defineNewDerivation("X-der", &d1)) {
		std::cout << "Cannot create derivation " << std::endl;
	}
	else {
		std::cout << d1->debugPrint() << std::endl;
	};
	

	//std::cout << "Hello phibar tests !" << std::endl;

}

