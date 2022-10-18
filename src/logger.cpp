#include "logger.h" 


namespace PsiBar {

	Ref<Logger> gDevErrLogger = CreateRef<Logger>("stderr");


	Logger::Logger(const std::string& filename)
	{
		if (filename == "stdout") {
			m_fileHandle = stdout;
		}
		else if (filename == "stderr") {
			m_fileHandle = stderr;
		}
		else {
			m_fileHandle = std::fopen(filename.c_str(), "a");
			if (!m_fileHandle) {
				m_fileHandle = stderr;
				Log(ErrorLevel::FATAL, "Cannot open log file %s .", filename);
			};

		};
	};
	
	Logger::~Logger()
	{
		std::fclose(m_fileHandle);
	}
};
