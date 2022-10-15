#pragma once
#include "util.h"

namespace PsiBar {

	class Derivation {

	public:

		Derivation(const std::string& name)
			:m_ID{ name } 
		{};

		inline std::string getName() { return m_ID; };
	private:

		std::string m_ID;
	};

	using DerTable = std::unordered_map<std::string, Ref<Derivation>>;

	enum class Parity {
		NONE = 0 ,
		EVEN = 1,
		ODD = -1,
	};


	class Generator {

	public:
		Generator(const std::string& ID)
			:m_ID{ ID }
		{};

		inline void setPairy(Parity parity) { m_parity = parity; };
		inline void	setIsScalar(bool isScalar) { m_isScalar = isScalar;  };
		
		int addDerivation(Ref<Derivation> der);
		int delDerivation(const std::string& name);
		bool isActingDerivation(const std::string& name);

		inline DerTable getActingDerivations() { return m_actedDerList; };
		

	private:
		std::string m_ID;

		/* If the generator is a scalar, then It can be passed through any derivation. */
		bool m_isScalar = false;

		Parity m_parity = Parity::NONE;
		DerTable m_actedDerList;


	};

	class Symbol :public Generator {

	public:

		Symbol(const std::string& name)
			: Generator{ name }
		{

		};

	private:
		//PropTable m_symbProp;
	};

};