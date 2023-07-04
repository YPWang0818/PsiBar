#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdio>
#include <functional>
#include <string_view>
#include <cctype>
#include <cstdint>
#include <exception>
#include <format>


#include "boost/unordered_map.hpp"
#include "boost/container/vector.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/container/deque.hpp"
#include "boost/regex.hpp"

#ifndef PSIBAR_DEBUG
#define PSIBAR_DEBUG
#endif


namespace PsiBar {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	};


	template<typename U>
	using Ref = std::shared_ptr<U>;

	template<typename U, typename ... Args>
	constexpr Ref<U> CreateRef(Args&& ... args) {
		return std::shared_ptr<U>(new U(std::forward<Args>(args)...));

	};


	enum ErrorCode {
		OK = 0,
		TRACE,
		WARN,
		ERROR,
		CRITICAL
	};

	class Error {
	public:

		template<typename ... Args>
		Error(ErrorCode errcode, std::string msg, Args ... args)
			:m_code{ errcode }
		{
			std::stringstream ss;
			ss << std::vformat(msg, std::make_format_args(args...));
			m_msg = ss.str();
		};
	

		Error(std::string msg)
			: m_msg{ msg }, m_code{ERROR}
		{};

		Error()
			: m_msg{}, m_code{OK}
		{}

		inline int code() { return m_code; }
		inline std::string msg() { return m_msg; };

	private:

		std::string m_msg;
		ErrorCode m_code;
	};


	



	


};