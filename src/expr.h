#pragma once

#include "util.h"

namespace PsiBar {

	struct Expr;

	class Derivation {

	public:

		Derivation(const std::string& name)
			:m_ID{ name }, m_suffix{ nullptr }, m_hasSuffix{false}
		{};
		
		Derivation(const std::string_view name)
			:m_ID{ name }, m_suffix{nullptr}, m_hasSuffix{ false }
		{
		};

		inline std::string getName() { return m_ID; };

		inline void setHasSuffix(bool t) { m_hasSuffix = t; };
		inline bool hasSuffix() { return m_hasSuffix; };

		inline void setSuffix(Ref<Expr> expr) {
			if(m_hasSuffix) m_suffix = expr;
		}

		inline Ref<Expr> getSuffix() {
			return m_suffix;
		}




#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 
	
	private:

		Ref<Expr> m_suffix;
		bool m_hasSuffix;
		std::string m_ID;
	};

	using DerTable = std::unordered_map<std::string, Ref<Derivation>>;

	enum class Parity {
		NONE = 0 ,
		EVEN = 1,
		ODD = -1,
	};



	struct Expr;
	typedef std::unordered_map<std::string, Ref<Expr> > ArgsTb;

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
		
		inline bool haveArg(std::string_view arg) {
			return (m_args.find(std::string{ arg }) == m_args.end()) ? false : true;
		};

		std::string setArg(const std::string& arg, Ref<Expr> = nullptr);
		Ref<Expr> getArg(const std::string& arg);



		//inline ArgsTb args() { return m_args; };


#ifdef PSIBAR_DEBUG
		std::string debugPrint();
#endif 


	private:
		std::string m_ID;

		/* If the generator is a scalar, then It can be passed through any derivation. */
		bool m_isScalar = false;
		Parity m_parity = Parity::NONE;
		ArgsTb m_args;


	};

	enum  ExprType {
		NAT,
		REAL,
		GEN,
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


#ifdef	PSIBAR_DEBUG
		
		std::string debugPrint(int indentLevel = 0);
	private:

		std::string debugPrintSubExpr(int indentLevel = 0);

#endif

	};

};