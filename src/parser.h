#pragma once
#include "util.h"
#include "expr.h"
#include "logger.h"

namespace PsiBar {


	

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


	template<typename T>
	class Parser {

	public:

		Parser();

		Ref<T> parseInput(std::string_view src);
		void Reset();



	protected:

		std::string_view nextToken(std::size_t count = 1); // consume one token from the token buffer. @count indicate the number of token consumed, but only the last token is returned.
		std::string_view lookToken(std::size_t step = 1); // Only look at the token buffer. @step the number of tokens to look ahead.  
		std::string_view getToken(); // Actually find the next @count tokens and put in the button of the token buffer stack.

		// The top level parser.
		virtual void parseFull() = 0;

		void push(Ref<T> gen = nullptr) {
			m_stack.push_back(gen);
		};

		Ref<T> pop();

		void createNode(Ref<T> node);

		void onError(std::string msg);


	protected:

		std::string_view m_srcView;
		boost::container::vector<Ref<T>>  m_stack;
		boost::container::deque<std::string_view> m_tokenBuffer;
		size_t m_idx;


		const size_t m_tokenBufferSz = 10;
		const size_t m_maxTagSz = 128;


	};

	template<typename T>
	Ref<T> Parser<T>::parseInput(std::string_view src) {

		try {

			Reset();

			m_srcView = src;

			for (int i = 0; i < m_tokenBufferSz; i++) {
				m_tokenBuffer.push_back(getToken());
			};

			parseFull();
			return pop();
		}
		catch (ParseException e) {

			PSIBAR_ERR(e.what());
			return nullptr;
		}
	};




	class ExprParser :public  Parser<Expr> {

	public:
		ExprParser();

	private:


		void parseFull() override;

		void parseExpression();
		void parseTerm();
		void parseFactor();
		void parseGenerator();
		void parseDerFactor();

		bool isNat(std::string_view token, int64_t* value = nullptr);
		bool isReal(std::string_view token, double* value = nullptr);
		bool isId(std::string_view token);


		// This function will push the expression parsed directly into the argument slot of gen. 
		void parseGenProp(Ref<Function> gen);

		// These function impelments a token stream from the source, which will be consumed by the recursive decent parser.

	};

}