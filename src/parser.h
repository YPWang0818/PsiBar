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

		inline std::size_t getPosition() {
			
			const char* end = lookToken().data();
			const char* start = m_srcView.data();

			return end - start;
		
		};

		inline std::string_view getSrcView() { return m_srcView; };


		bool isNat(std::string_view token, int64_t* value = nullptr);
		bool isReal(std::string_view token, double* value = nullptr);
		bool isId(std::string_view token);

	protected:

		std::string_view m_srcView;
		boost::container::vector<Ref<T>>  m_stack;
		boost::container::deque<std::string_view> m_tokenBuffer;
		size_t m_idx;
		size_t m_curPos;


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


	template<typename T>
	Parser<T>::Parser() { Reset(); }

	template<typename T>
	Ref<T> Parser<T>::pop() {

		if (m_stack.empty()) return nullptr;

		Ref<T> res = m_stack.back();
		m_stack.pop_back();

		return res;
	}


	template<typename T>
	void Parser<T>::createNode(Ref<T> node)
	{
		if (m_stack.empty()) return;

		while (m_stack.back() != nullptr) {

			node->exprs.push_back(m_stack.back());
			m_stack.pop_back();
		};
		pop(); push(node);
		return;
	}




	template<typename T>
	void Parser<T>::onError(std::string msg)
	{
		throw ParseException(msg);
	};

	template<typename T>
	void Parser<T>::Reset()
	{
		m_srcView = std::string_view{ nullptr, 0 };
		m_stack.clear();
		m_tokenBuffer.clear();
		m_idx = 0;
		m_curPos = 0;

	}



	template<typename T>
	std::string_view Parser<T>::nextToken(std::size_t count)
	{
		m_tokenBuffer.push_back(getToken());
		std::string_view& res = m_tokenBuffer.front();
		m_tokenBuffer.pop_front();

		for (int c = 1; c < count; c++) {

			m_tokenBuffer.push_back(getToken());
			res = m_tokenBuffer.front();
			m_tokenBuffer.pop_front();
		};

		return res;
	}


	template<typename T>
	std::string_view Parser<T>::lookToken(std::size_t step)
	{
		// For effeciency, we don't check the bound here. ( Bad idea?)

		return m_tokenBuffer[(step - 1)];
	}


	template<typename T>
	std::string_view Parser<T>::getToken()
	{
		// Iterating over the spaces and comments.
		for (; m_idx < m_srcView.size() && (isspace(m_srcView[m_idx]) || m_srcView[m_idx] == ';'); m_idx++) {
			if (m_srcView[m_idx] == ';') while (m_idx < m_srcView.size() && m_srcView[m_idx] != '\n') {
				m_idx++;
			};
		};


		// End of file, return an empty token pointing to the end of the source.
		if (m_idx >= m_srcView.size()) { return m_srcView.substr(m_srcView.size(), 0); };

		std::string_view token;


		// The token is the special character '(' and ')'. This is to prevent code such as "(+ " or "(variable)" to be
		// identified as one token.
		if (m_srcView[m_idx] == '(' || m_srcView[m_idx] == ')') {

			token = m_srcView.substr(m_idx, 1); m_idx++;
			return token;

		};


		// The iterate through the token, which is deliminated by an isspace() character.
		size_t forward = 0;
		for (;
			((m_idx + forward) < m_srcView.size()) &&
			!isspace(m_srcView[m_idx + forward]) &&
			m_srcView[m_idx + forward] != ';' &&
			m_srcView[m_idx + forward] != '(' &&
			m_srcView[m_idx + forward] != ')'	// To avoid code such as +) registored as one token.
			; forward++) {
		};

		token = m_srcView.substr(m_idx, forward);
		m_idx += forward;
		return token;
	};

	template<typename T>
	bool Parser<T>::isId(std::string_view token)
	{
		boost::regex idreg{ "[\\w][\\w\\-]*\\:?" };
		//boost::regex idreg{ "[a-zA-Z]*" };

		return boost::regex_match(std::string{ token }, idreg);
	};


	template<typename T>
	bool Parser<T>::isNat(std::string_view token, int64_t* value)
	{


		// Should use regular expression in future when dealing with arbitrary percision integers. 

		int64_t result, i;
		std::sscanf(std::string{ token }.c_str(), "%lli%lln", &result, &i);

		// Only match if scanf consumes the whole token.
		if (i != token.size()) { return false; };

		*value = result;
		return true;
	};

	template<typename T>
	bool Parser<T>::isReal(std::string_view token, double* value)
	{

		int64_t i; double result;

		std::sscanf(std::string{ token }.c_str(), "%lg%lln", &result, &i);


		if (i != token.size()) { return false; };

		*value = result;
		return true;

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



		// This function will push the expression parsed directly into the argument slot of gen. 
		void parseGenProp(Ref<Generator> gen);

		// These function impelments a token stream from the source, which will be consumed by the recursive decent parser.

	};

	enum class CommandType {
		UNDEF,
		HELP,
		SETGEN, // Defining a new generator    
		SETDER, // Defining a new derivation.  
		LOCAL   // List local variables.
	};


	struct InputCommand {

		CommandType type = CommandType::UNDEF;
		boost::container::vector<std::string> arguments;
		std::string_view rest;   // The unparsed part of the input.

		#ifdef PSIBAR_DEBUG
		std::string debugPrint();
		#endif 
	};


	class CommandParser : public Parser<InputCommand> {

	public:
		CommandParser() {};
	private:
		void parseFull() override;
	};


	class DerivationParser : public Parser<Derivation> {

	public:
		DerivationParser() {};
	private:
		void parseFull() override;

	};

	class GeneratorParser : public Parser<Generator> {
	public:
		GeneratorParser() {};
	private:
		void parseFull() override;
		void parseGenProp(Ref<Generator> gen);
		void parsePType(Ref<Generator> gen);

	};





}