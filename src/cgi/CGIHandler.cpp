#include "CGIHandler.hpp"
#include <algorithm>

CGIHandler::CGIHandler(const std::map<std::string, std::string>& env, const std::string& body, const std::vector<std::string>& cgiPaths)
	: _envMap(env), _cgiPaths(cgiPaths), _requestBody(body), _env(0), _argv(0) {

	_cgiPath = cgiPaths[0]; // use first CGI path as default
	_envSize = env.size();
	_env = new char*[_envSize + 1];
	_env[_envSize] = NULL;

	setEnv(env);
}

CGIHandler::~CGIHandler() {
	cleanEnv();
	if (_argv) {
		if (_argv[0])
			free(_argv[0]);
		delete[] _argv;
	}
}

void CGIHandler::setEnv(const std::map<std::string, std::string>& env) {
	size_t i = 0;
	try {
		for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it, ++i) {
			std::string entry = it->first + "=" + it->second;
			_env[i] = strdup(entry.c_str());
			if (!_env[i])
				throw std::bad_alloc();
		}
	} catch (...) {
		cleanEnv();
		throw;
	}
}

void CGIHandler::cleanEnv() {
	for (size_t i = 0; i < _envSize; ++i) {
		if (_env[i])
			free(_env[i]);
	}
	delete[] _env;
	_env = 0;
}

std::string CGIHandler::getFileExtension(const std::string& filename) {
	size_t dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos) {
		return filename.substr(dotPos + 1);
	}
	return "";
}

std::string CGIHandler::getCmd() {
	std::map<std::string, std::string>::const_iterator it = _envMap.find("PATH_INFO");
	if (it == _envMap.end())
		throw std::runtime_error("Missing PATH_INFO in environment map");

	std::string pathInfo = it->second;
	size_t lastSlash = pathInfo.find_last_of('/');
	std::string scriptName = (lastSlash != std::string::npos) ? pathInfo.substr(lastSlash + 1) : pathInfo;

	bool allowed = false; // confirm scriptName is in the allowed CGI list
	for (std::vector<std::string>::const_iterator vit = _cgiPaths.begin(); vit != _cgiPaths.end(); ++vit) {
		if (*vit == scriptName) {
			allowed = true;
			break;
		}
	}
	if (!allowed)
		throw std::runtime_error("Script not allowed by configuration");

	std::string fullPath = pathInfo[0] == '/' ? "." + pathInfo : pathInfo; // build potential absolute/relative path

	if (access(fullPath.c_str(), X_OK) != 0) // check file exists and is executable
		throw std::runtime_error("Script does not exist or is not executable: " + fullPath);

	return fullPath;
}

void CGIHandler::setArgs(const std::string& cmd) {
	_argv = new char*[2];
	_argv[0] = strdup(cmd.c_str());
	if (!_argv[0])
		throw std::bad_alloc();
	_argv[1] = NULL;
}

char** CGIHandler::getEnvCStrArr() const {
	char** env = new char*[_envSize + 1];
	int j = 0;
	for (std::map<std::string, std::string>::const_iterator i = _envMap.begin(); i != _envMap.end(); i++) {
		std::string element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string CGIHandler::addContentLength(const std::string& httpResponse) {
	size_t headerEndPos = httpResponse.find("\r\n\r\n");
	std::string headers = httpResponse.substr(0, headerEndPos);
	std::string body = httpResponse.substr(headerEndPos + 4);

	std::istringstream headersStream(headers);
	std::vector<std::string> headerLines;
	std::string line;
	while (std::getline(headersStream, line)) {
		if (!line.empty()) {
			headerLines.push_back(line);
		}
	}

	if (headers.find("Content-Length:") != std::string::npos)
		return httpResponse;

	std::ostringstream oSS;
	oSS << "Content-Length: " << body.size();
	headerLines.push_back(oSS.str());

	std::ostringstream modifiedHeaders;
	for (size_t i = 0; i < headerLines.size(); ++i) {
		modifiedHeaders << headerLines[i];
		if (i < headerLines.size() - 1) {
			modifiedHeaders << "\r\n";
		}
	}

	return modifiedHeaders.str() + "\r\n\r\n" + body;
}

std::string CGIHandler::Execute() {
	std::string cmd_ = getCmd();
	setArgs(cmd_);

	int fdResponse[2];
	int fdRequest[2];
	if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1)
		throw std::runtime_error("pipe() failed");

	pid_t pid = fork();
	if (pid == -1) {
		throw std::runtime_error("fork() failed");
	} else if (pid == 0) {
		if (dup2(fdResponse[1], STDOUT_FILENO) == -1 ||
			dup2(fdRequest[0], STDIN_FILENO) == -1) {
			perror("dup2");
			exit(1);
		}

		close(fdResponse[0]);
		close(fdResponse[1]);
		close(fdRequest[0]);
		close(fdRequest[1]);

		execve(_argv[0], _argv, _env);
		perror("execve");
		exit(1);
	} else {
		close(fdRequest[0]);
		write(fdRequest[1], _requestBody.c_str(), _requestBody.size());
		close(fdRequest[1]);
		close(fdResponse[1]);

		struct pollfd pfd;
		pfd.fd = fdResponse[0];
		pfd.events = POLLIN;

		int poll_ret = poll(&pfd, 1, 5000);
		if (poll_ret == 0) {
			kill(pid, SIGKILL);
			close(fdResponse[0]);
			waitpid(pid, 0, 0);
			throw std::runtime_error("CGI script timeout");
		} else if (poll_ret < 0) {
			kill(pid, SIGKILL);
			close(fdResponse[0]);
			waitpid(pid, 0, 0);
			throw std::runtime_error("poll() failed");
		}

		int status;
		waitpid(pid, &status, 0);

		char buffer_[1024];
		int ret = 0;
		std::string responseBody;
		while ((ret = read(fdResponse[0], buffer_, sizeof(buffer_))) > 0) {
			responseBody.append(buffer_, ret);
		}

		close(fdResponse[0]);
		return addContentLength(responseBody);
	}
}
