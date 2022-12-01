#pragma once
#include "util.h"

namespace PsiBar {

	class Derivation {

	public:

		Derivation(const std::string& name)
			:m_ID{ name } 
		{};
		
		Derivation(const std::string_view name)
			:m_ID {name}
		{
		};

		inline std::string getName() { return m_ID; };

#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 
	
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

		inline std::string getName() { return m_ID; };

		inline void setPairy(Parity parity) { m_parity = parity; };
		inline void	setIsScalar(bool isScalar) { m_isScalar = isScalar;  };
		
		int addDerivation(Ref<Derivation> der);
		int delDerivation(const std::string& name);
		bool isActingDerivation(const std::string& name);

		inline DerTable getActingDerivations() { return m_actedDerList; };


#ifdef PSIBAR_DEBUG
	protected:
		std::string debugPrintBase();
#endif 


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
#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 

	private:
		//PropTable m_symbProp;
	};

	enum  ExprType {
		NAT,
		REAL,
		GEN,
		DER,
		DERFACTOR,
		EXPR,
		TERM,
		FACTOR
	};

	struct Expr;


	typedef boost::container::vector<Ref<Expr>> ExprList;
	typedef Ref<Generator> GenRef;
	typedef Ref<Derivation> DerRef;

	struct Expr {

		Expr(ExprType tag)
			:tag{tag}
		{
			//ders{};
		};


		ExprType tag;

		union {
			ExprList exprs;
			DerRef der;
			GenRef gen;
			int64_t nat;
			double real;
		};

		~Expr() {
		
		};
	};

};