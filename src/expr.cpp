#include "psibar.h"
#include "expr.h"

namespace PsiBar {

	

	int Generator::addDerivation(Ref<Derivation> der)
	{
		if (m_actedDerList.find(der->getName()) == m_actedDerList.end()) {
			return W_RESOURCE_DUPICATE;
		}

		m_actedDerList[der->getName()] = der;
		return _OK;
	};

	int Generator::delDerivation(const std::string& name)
	{
		m_actedDerList.erase(name);

		return _OK;
	};


	bool Generator::isActingDerivation(const std::string& name)
	{
		if (m_actedDerList.find(name) == m_actedDerList.end()) {
			return false;
		};

		return true;
	};


#ifdef PSIBAR_DEBUG

	std::string Generator::debugPrintBase()
	{

		std::stringstream ss;

		ss << "Parity: " << std::to_string((int)m_parity) << "\n";
		ss << "Is scalar: " << std::to_string(m_isScalar) << "\n";

		ss << "Acted derivation List: ";

		for (auto d : m_actedDerList) {
			ss << d.first << ",";
		};
		ss << "\n";

		return ss.str();
	};


	std::string Derivation::debugPrint()
	{

		std::stringstream ss;

		ss << "[DERIVATION] " << this->getName() << ":\n";
	
		return ss.str();
	};


	std::string Symbol::debugPrint()
	{
		std::stringstream ss;

		ss << "[SYMBOL] " << this->getName() << ":\n";
		ss << this->debugPrintBase();

		return ss.str();
	};



#endif
};