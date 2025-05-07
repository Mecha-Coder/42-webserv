#pragma once

#include <iostream>
#include <unistd.h>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdexcept>
#include <fcntl.h>
#include <signal.h>

class CGIHandler {
	public:
		CGIHandler(const std::map<std::string, std::string>& env, const std::string& body, const std::vector<std::string>& cgiPaths);
		~CGIHandler();
	
		void setEnv(const std::map<std::string, std::string>& env);
		void cleanEnv();
		std::string getFileExtension(const std::string& filename);
		std::string getCmd();
		void setArgs(const std::string& cmd);
		char** getEnvCStrArr() const;
		std::string addContentLength(const std::string& httpResponse);
		std::string Execute();
	private:
		std::map<std::string, std::string> _envMap;
		std::vector<std::string> _cgiPaths;
		std::string _requestBody;
		char** _env;
		size_t _envSize;
		char** _argv;
		std::string _cgiPath;
};
