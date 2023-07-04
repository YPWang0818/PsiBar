#pragma once
#include "util.h"
#include "expr.h"
#include "parser.h"

namespace PsiBar {

	using SymbTb = std::unordered_map<std::string, Ref<Generator>>;
	using DerTb = std::unordered_map<std::string, Ref<Derivation>>;


	class Program {

	public:

		static Program& getContext() {
			static Program s_instance;
			return s_instance;
		}

		Program(Program const&) = delete;
		void operator=(Program const&) = delete;

	
		//inline Ref<SymbTb> symbolsTable() { return m_symbolsTable; };
		//inline Ref<DerTb> derivationsTable() { return m_derivationsTable; };

		Error defGenerator(const std::string& cmd, Ref<Generator>& rest);
		Error defDerivation(const std::string& cmd, Ref<Derivation>& rest);


	private:

		SymbTb m_symbolsTable;
		DerTb m_derivationsTable;
		
		ExprParser m_exprParser;		
		GeneratorParser m_genParser;
		DerivationParser m_derParser;

		::boost::container::vector<ErrorCode> m_errStack;

		Program() {};
		~Program() {};


	};


	

	/*
	
	int defineNewDerivation(
		const std::string& name,
		Ref<Derivation>* der = NULL);
	*/

	

}