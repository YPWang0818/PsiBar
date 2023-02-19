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

	struct ExprParser;

	typedef std::function<void(ExprParser*)> ErrCallback;


	struct ExprParser {
		
		ExprParser() {


			Reset();
		}
		

		Ref<Expr> Parse(const std::string& src);


		/* Reset the parser to a state to parse a new source.This function is called in the beginning of parse.
		 So one don't ususally need to rum them explicitly. */

		void Reset();

		/* These functions implements a recursive decent parser.The side effect of each(except for one) function  will result in the AST of the corresponding experssion being consturctedand put on top of the			expression stack. */

		void parseExpression();
		void parseTerm();
		void parseFactor();
		void parseGenerator();
		void parseDerFactor();

		// This function will push the expression parsed directly into the argument slot of gen. 
		void parseGenProp(Ref<Function> gen); 

		// These function impelments a token stream from the source, which will be consumed by the recursive decent parser.
	
		std::string_view nextToken(std::size_t count = 1); // consume one token from the token buffer. @count indicate the number of token consumed, but only the last token is returned.
		std::string_view lookToken(std::size_t step = 1); // Only look at the token buffer. @step the number of tokens to look ahead.  
		std::string_view getToken(); // Actually find the next @count tokens and put in the button of the token buffer stack.


		void createNode(Ref<Expr> node);
		void push(Ref<Expr> gen = nullptr);
		Ref<Expr> pop();

		bool isNat(std::string_view token, int64_t* value = nullptr);
		bool isReal(std::string_view token, double* value = nullptr);
		bool isId(std::string_view token);
		

		
		// If there is no callback function, the error message is printed and the program halts.
		// Otherwise the callback is called and the error message is ignored. 

		void onError(std::string msg, ErrCallback callback = nullptr);


		std::string_view m_srcView;
		ExprStack m_stack;
		boost::container::deque<std::string_view> m_tokenBuffer;
		size_t m_idx;


		const size_t m_tokenBufferSz = 10;
		const size_t m_maxTagSz  = 128;
		bool m_errorFlag = false;

	};


	class ParseException : public std::exception {
	private:

		std::string message;

	public:
		ParseException(const std::string& msg) : message(msg) {
		};


		char* what() {
			return (char*)message.c_str();
		}
	};







}