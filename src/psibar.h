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
        void operator=(GlobalContext const&) =delete;

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
		Ref<Symbol> *symb = NULL,
		Parity parity = Parity::NONE,
		bool scalar = false );



	int defineNewDerivation(
		const std::string& name,
		Ref<Derivation>* der = NULL );


	
	typedef ::boost::container::vector<Ref<Expr>>  ExprStack;

	struct ExprParser {

		//ExprParser(const std::string& src);

		static int Parse(const std::string& src, Ref<Expr>& output);
	private:
		static int parseExpression(std::string_view src, std::size_t idx);
		static int parseTag(std::string_view src, std::size_t idx);

		static int flushStack();
		static void push(Ref<Expr> gen);

		static bool isTagGen(std::string_view tag);
		static bool isTagNat(std::string_view tag);
		static bool isTagReal(std::string_view tag);


		inline static ExprStack m_stack;
		inline static int m_bp, m_sp;

	};






}