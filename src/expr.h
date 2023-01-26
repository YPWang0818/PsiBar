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

		Generator(std::string_view ID) 
			:m_ID{ std::string{ID} }
		{}

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
		{};

		Symbol(std::string_view name)
			: Generator{ name }
		{};


#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 

	private:
		//PropTable m_symbProp;
	};


	struct Expr;
	typedef std::unordered_map<std::string, Ref<Expr> > ArgsTb;

	class Function : public Generator {


	public:
		Function(const std::string& name)
			: Generator{ name }
		{};

		Function(std::string_view name)
			: Generator{ name }
		{};


		inline bool haveTag(std::string_view tag) { 

			// Copying the string_view to string, not ideal...
			 return (m_args.find(std::string{ tag }) ==  m_args.end() )? false: true;
		};

		inline ArgsTb args() { return m_args; };
	

#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 

	private:
		ArgsTb m_args;

	};





	enum  ExprType {
		NAT,
		REAL,
		GEN,
		//DER,
		DERFACTOR,
		EXPR,
		TERM,
		FACTOR
	};


	typedef boost::container::vector<Ref<Expr>> ExprList;
	typedef Ref<Generator> GenRef;
	typedef Ref<Derivation> DerRef;

	struct Expr {

		Expr(ExprType tag)
			:tag{ tag }, exprs()
		{
		}

		Expr(DerRef der, uint64_t pow) 
			:tag{ ExprType::DERFACTOR }, derFactor{der, pow}
		{};

		Expr(GenRef gen)
			:tag{ ExprType::GEN }, gen{gen}
		{};

		Expr(int64_t nat) 
			:tag{ ExprType::NAT }, nat{ nat }
		{};


		Expr(double real)
			:tag{ ExprType::REAL }, real{real }
		{};



		ExprType tag;

		union {
			boost::tuple<DerRef, uint64_t> derFactor;
			ExprList exprs;
			GenRef gen = nullptr;
			int64_t nat;
			double real;
		};

		

		~Expr() {
		
		};
	};

};