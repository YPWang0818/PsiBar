#include "phibar.h"

namespace PsiBar {

	struct GlobalContext {

		SymbTb symbolTable;
		DerTb derivationTable;

	} gContext;

	void init()
	{
	};


	int defineNewSymbol(const std::string& name, Ref<Symbol>* symb, Parity parity, bool scalar)
	{

		if (gContext.symbolTable.find(name) != gContext.symbolTable.end()) {
			return W_RESOURCE_DUPICATE;
		};

		Ref<Symbol> syb = CreateRef<Symbol>(name);

		syb->setIsScalar(scalar);
		syb->setPairy(parity);

		if (symb) *symb = syb;
		gContext.symbolTable[name] = syb;

		return _OK;
	}

	int defineNewDerivation(const std::string& name, Ref<Derivation>* der)
	{

		if (gContext.derivationTable.find(name) != gContext.derivationTable.end()) {
			return W_RESOURCE_DUPICATE;
		};

		Ref<Derivation> derivation = CreateRef<Derivation>(name);

		if (der) *der = derivation;
		gContext.derivationTable[name] = derivation;

		return _OK;
	}
};

