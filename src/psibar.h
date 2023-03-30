#pragma once
#include "util.h"
#include "expr.h"

namespace PsiBar {

	using SymbTb = std::unordered_map<std::string, Ref<Symbol>>;
	using DerTb = std::unordered_map<std::string, Ref<Derivation>>;

	class GlobalContext {

	public:

		static GlobalContext& getContext() {
			static GlobalContext s_instance;
			return s_instance;
		}

		GlobalContext(GlobalContext const&) = delete;
		void operator=(GlobalContext const&) = delete;

		Ref<SymbTb> symbolTable;
		Ref<DerTb> derivationTable;

	private:
		GlobalContext();

	};

	#define GET_SYMTB() GlobalContext::getContext().symbolTable 
	#define GET_DERTB() GlobalContext::getContext().derivationTable 



	void init();

	int defineNewSymbol(
		const std::string& name,
		Ref<Symbol>* symb = NULL,
		Parity parity = Parity::NONE,
		bool scalar = false);



	int defineNewDerivation(
		const std::string& name,
		Ref<Derivation>* der = NULL);


	

}