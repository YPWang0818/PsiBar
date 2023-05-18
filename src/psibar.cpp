#include "logger.h"
#include "psibar.h"


namespace PsiBar {


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
		exprParser = CreateRef<ExprParser>();
	}





};

