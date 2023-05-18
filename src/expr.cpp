
#include "expr.h"

namespace PsiBar {

	
	std::string Generator::setArg(const std::string& arg, Ref<Expr> init)
	{
		m_args[arg] = init;
		return arg;
	}

	Ref<Expr> Generator::getArg(const std::string& arg){

		auto res = m_args.find(arg);

		if (res != m_args.end()) {
			return res->second;
		} 
		return nullptr;
	}



#ifdef PSIBAR_DEBUG

	

	std::string Generator::debugPrint()
	{

		std::stringstream ss;

		ss << "Parity: " << std::to_string((int)m_parity) << "\n";
		ss << "Is scalar: " << std::to_string(m_isScalar) << "\n";

		ss << "Generator properties: \n";
		
		for (auto d : m_args) {
			ss << d.first << " , " <<  (d.second == nullptr ? "nil" : d.second->debugPrint()) << "\n";
		};

		ss << "\n";

		return ss.str();
	};



	std::string Derivation::debugPrint()
	{

		std::stringstream ss;

		ss << "[DERIVATION] " << this->getName() << " : \n";

		ss << "Has suffix: " << (this->hasSuffix() ? "true" : "false") << "\n";

		if (this->hasSuffix() && (this->getSuffix() != nullptr)) {
			ss << "Suffix: \n" << this->getSuffix()->debugPrint();
		};
	
		return ss.str();
	};


	std::string Expr::debugPrint(int indentLevel) {

		std::stringstream ss; int lvl = indentLevel;
		ss  << std::string(lvl, '  ');

		switch (tag) {
		case ExprType::NAT:
			ss  << "nat: " << nat;

			break;
		case ExprType::REAL:
			ss << "real: " << real;

			break;
		case ExprType::GEN :
			ss << "gen: " << gen->getName();

			break;
		case ExprType::DERFACTOR:
			ss << "defactor: " << derFactor.get<1>() << " , " << derFactor.get<0>()->debugPrint();

			break;
		case ExprType::EXPR:
			ss << "expr: " << debugPrintSubExpr(lvl);

			break;
		case ExprType::TERM:
			ss << "term: " << debugPrintSubExpr(lvl);

			break;
		case ExprType::FACTOR:
			ss << "factor: " << debugPrintSubExpr(lvl);

			break;
		};

		return ss.str();
	}
	std::string Expr::debugPrintSubExpr(int indentLevel)
	{
		std::stringstream ss;
		ss << "\n";

		for (auto s : exprs) {
			ss << s->debugPrint(indentLevel + 1) << "\n";
		};

		return ss.str();
	};




#endif

};