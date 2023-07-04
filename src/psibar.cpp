#include "logger.h"
#include "psibar.h"


namespace PsiBar {

	/*
	int defineNewDerivation(const std::string& name, Ref<Derivation>* der)
	{

		Program& p = Program::getContext();

		if (p.derivationsTable()->find(name) != p.derivationsTable()->end()) {
			return ;
		};

		Ref<Derivation> derivation = CreateRef<Derivation>(name);

		if (der) *der = derivation;
		(*p.derivationsTable())[name] = derivation;

		return _OK;
	};
	*/


	Error Program::defGenerator(const std::string& cmd, Ref<Generator>& rest)
	{
		m_genParser.Reset();
		Ref<Generator> gen = nullptr;
		Error e = m_genParser.parseInput(cmd, gen);

		if (e.code() > ErrorCode::WARN) {
			return e;
		};

		if (m_symbolsTable.find(gen->getName()) != m_symbolsTable.end()) {

			m_symbolsTable[gen->getName()] = gen;
			rest = gen;
			return Error(ErrorCode::WARN, "Generator {} already existed and it is replaced.", gen->getName());

		}
		else {
			m_symbolsTable[gen->getName()] = gen;
			rest = gen;
			return Error{};
		}
	

	};

	Error Program::defDerivation(const std::string& cmd, Ref<Derivation>& rest)
	{
		m_derParser.Reset();
		Ref<Derivation> der = nullptr;
		Error e = m_derParser.parseInput(cmd, der);

		if (e.code() > ErrorCode::WARN) {
			return e;
		}

		if (m_derivationsTable.find(der->getName()) != m_derivationsTable.end()) {

			m_derivationsTable[der->getName()] = der;
			rest = der;
			return Error(ErrorCode::WARN, "Derivation {} already existed and it is replaced.", der->getName());

		}
		else {
			m_derivationsTable[der->getName()] = der;
			rest = der;
			return Error{};
		};


	};




};

