#pragma once
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

			std::string banner;
			std::string colorcode;

			switch (lvl) {
			case ErrorLevel::FATAL:
				banner = "FATAL: ";
				colorcode = "\x1b[1;31m"; //Bold red.
				break;
			case ErrorLevel::ERROR:
				banner = "ERROR: "; 
				colorcode = "\x1b[31m"; // Red.
				break;
			case ErrorLevel::WARN:
				banner = "WARN: ";
				colorcode = "\x1b[33m"; // Yellow.
				break;
			case ErrorLevel::INFO:
				banner = "INFO: ";
				colorcode = "\x1b[36m"; // Cyan.
				break;
			case ErrorLevel::TRACE:
				banner = "TRACE: ";
				colorcode = "\x1b[2;37m";// Dimmed White.
				break;
			
			}

	
			if (m_fileHandle == stdout || m_fileHandle == stderr) {
				std::fprintf(m_fileHandle, colorcode.c_str());
			};

			std::fprintf(m_fileHandle, banner.c_str());
			std::fprintf(m_fileHandle, fmt.c_str(), arg ...);
			std::fprintf(m_fileHandle, "\n");

			if (m_fileHandle == stdout || m_fileHandle == stderr) {
				std::fprintf(m_fileHandle, "\x1b[0m");
			};
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


#ifdef WIN32
	#define PSIBAR_DBGBREAK() __debugbreak()
#else
	// Not impelmented yet.
#endif

#define PASIBAR_BREAK(...) {  PSIBAR_FATAL( __VA_ARGS__) PSIBAR_DBGBREAK(); };
#define PSIBAR_ASSERT(x, ...) {if(!x) { PSIBAR_FATAL( __VA_ARGS__) PSIBAR_DBGBREAK(); } };



}
