#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdio>
#include <functional>

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


	enum Errocode {
		_OK = 0,
		E_MISUSE,
		E_RESOURCE_NOT_FOUND,
		W_RESOURCE_DUPICATE
	};

	


};