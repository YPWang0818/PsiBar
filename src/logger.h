#include "util.h"

namespace PsiBar {

	enum class ErrorLevel {
		TRACE,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	class Logger {

	public:

		Logger(const std::string& filename) ;
		~Logger();

		template<typename ... Args>
		void Log(ErrorLevel lvl, const std::string& fmt, const Args& ... arg) {
			std::fprintf(m_fileHandle, fmt.c_str(), arg ...);
		};
	private:
		
		std::FILE* m_fileHandle;
		std::string m_fileName;

	};

	extern Ref<Logger> gDevErrLogger;

#ifdef PSIBAR_DEBUG

	#define PSIBAR_ERR(...)  ::PsiBar::gDevErrLogger->Log(::PsiBar::ErrorLevel::ERROR, __VA_ARGS__);
	#define PSIBAR_INFO(...)  ::PsiBar::gDevErrLogger->Log(::PsiBar::ErrorLevel::INFO, __VA_ARGS__);
	#define PSIBAR_WARN(...)  ::PsiBar::gDevErrLogger->Log(::PsiBar::ErrorLevel::WARN, __VA_ARGS__);

	#define PSIBAR_TRACE(...)  ::PsiBar::gDevErrLogger->Log(::PsiBar::ErrorLevel::TRACE, __VA_ARGS__);
	#define PSIBAR_FATAL(...)  ::PsiBar::gDevErrLogger->Log(::PsiBar::ErrorLevel::FATAL, __VA_ARGS__);

#elif

	// Disable all log msg.
	#define PSIBAR_ERR(...)  
	#define PSIBAR_INFO(...)  
	#define PSIBAR_WARN(...) 
	#define PSIBAR_TRACE(...)  
	#define PSIBAR_FATAL(...)  

#endif

#define PASIBAR_BREAK(...) { PSIBAR_FATAL( __VA_ARGS__) };
#define PSIBAR_ASSERT(x, ...) {if(!x) PSIBAR_FATAL( __VA_ARGS__)};

}
