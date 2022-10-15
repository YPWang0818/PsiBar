#pragma once
#include "util.h"
#include "expr.h"

namespace PsiBar {
	
	using SymbTb = std::unordered_map<std::string, Ref<Symbol>>;
	using DerTb = std::unordered_map<std::string, Ref<Derivation>>;



	void init();

	int defineNewSymbol(
		const std::string& name,
		Ref<Symbol> *symb = NULL,
		Parity parity = Parity::NONE,
		bool scalar = false );



	int defineNewDerivation(
		const std::string& name,
		Ref<Derivation>* der = NULL );

}