#include <iostream>

#include "src/expr.h"
#include "src/psibar.h"
#include "src/logger.h"

void main(int argc, char* argv[]) {

	using namespace PsiBar;

	Ref<Symbol> s1;
	Ref<Derivation> d1;

	if (defineNewSymbol("alpha", &s1, Parity::EVEN, true)) {
		PSIBAR_ERR("Cannot create symbol %s", "alpha");
	} else {
		PSIBAR_FATAL("%s", s1->debugPrint().c_str());
	};

	if (defineNewDerivation("X-der", &d1)) {
		PSIBAR_ERR("Cannot create derivation %s", "X-der");
	}
	else {
		PSIBAR_WARN("%s", d1->debugPrint().c_str());
	};
	

	//std::cout << "Hello phibar tests !" << std::endl;

}

