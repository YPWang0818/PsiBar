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


		static int Parse(const std::string& src, Ref<Expr>& output);
	private:

		// These functions implements a recursive decent parser. The side effect of each (except for one) function will
		// result in the AST of the corresponding experssion being consturcted and put on top of the expression stack.

		static void parseExpression(std::string_view src);
		static void parseTerm(std::string_view src);
		static void parseFactor(std::string_view src);
		static void parseGenerator(std::string_view src);
		static void parseDerFactor(std::string_view src);

		// This function will push the expression parsed directly into the argument slot of gen. 
		static void parseGenProp(std::string_view src, Ref<Function> gen); 

		// These function impelments a token stream from the source, which will be consumed by the recursive decent parser.
	
		static std::string_view nextToken(std::string_view src); // consume one token from the token buffer.
		static std::string_view lookToken(std::string_view src, std::size_t step = 1); // Only look a the token buffer.
		static std::string_view getToken(std::string_view src); // Actually find the next token from @src, and put in the button of the stack. 


		static void createNode(Ref<Expr> node);
		static void push(Ref<Expr> gen = nullptr);
		static Ref<Expr> pop();

		static bool isNat(std::string_view token, int64_t* value = nullptr);
		static bool isReal(std::string_view token, double* value = nullptr);
		static bool isId(std::string_view token);
		


		inline static ExprStack m_stack;
		inline static boost::container::deque<std::string_view> m_tokenBuffer;

		inline static size_t m_idx;

		inline static const size_t m_tokenBufferSz = 4;
		inline static const size_t m_maxTagSz  = 128;
	};






}