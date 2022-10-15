#include "phibar.h"
#include "expr.h"

namespace PsiBar {


	int Generator::addDerivation(Ref<Derivation> der)
	{
		if (m_actedDerList.find(der->getName()) == m_actedDerList.end() ) {
			return W_RESOURCE_DUPICATE;
		}

		m_actedDerList[der->getName()] = der;
		return _OK;
	}

	int Generator::delDerivation(const std::string& name)
	{
	
		m_actedDerList.erase(name);

		return _OK;
	}


	bool Generator::isActingDerivation(const std::string& name)
	{
		if (m_actedDerList.find(name) == m_actedDerList.end()) {
			return false;
		};

		return true;
	}
};