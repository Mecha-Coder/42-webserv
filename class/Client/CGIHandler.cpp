/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:09:56 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/26 10:46:51 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CGIHandler.hpp"
#include <algorithm>

CGIHandler* CGIHandler::Create(
	const std::map<std::string, std::string>& env,
	const std::string& body,
	const std::vector<std::string>& cgiPaths) {
	CGIHandler* handler = NULL;
	try {
		handler = new CGIHandler(env, body, cgiPaths);
		return (handler);
	} catch (...) {
		std::cerr << "CGIHandler Factory failed before construction.\n";
		delete handler;
		return NULL;
	}
}

CGIHandler::CGIHandler(const std::map<std::string, std::string>& env,
	const std::string& body,
	const std::vector<std::string>& cgiPaths)
	: _envMap(env), _cgiPaths(cgiPaths), _requestBody(body), _argv(0) {
	if (cgiPaths.empty()) {
		throw std::runtime_error("cgiPaths is empty");
	}
	_cgiPath = cgiPaths[0];
	_env.Set(_envMap);
}

CGIHandler::~CGIHandler() {
	resetArgs();
	// resetEnv();
}

void CGIHandler::resetArgs() {
	if (_argv) {
		if (_argv[0])
			free(_argv[0]);
		delete[] _argv;
		_argv = 0;
	}
}

// void CGIHandler::resetEnv() {
// 	if (_env) {
// 		for (size_t i = 0; i < _envSize; ++i)
// 			free(_env[i]);
// 		delete[] _env;
// 		_env = 0;
// 	}
// }

// void CGIHandler::setEnv(Client& client, const std::map<std::string, std::string>& env) {
// 	resetEnv();
// 	_envSize = env.size();
// 	_env = new char*[_envSize + 1];
// 	for (size_t i = 0; i <= _envSize; ++i)
// 		_env[i] = 0;

// 	size_t i = 0;
// 	try {
// 		for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it, ++i) {
// 			std::string entry = it->first + "=" + it->second;
// 			_env[i] = strdup(entry.c_str());
// 			if (!_env[i])
// 				throw std::bad_alloc();
// 		}
// 	} catch (...) {
// 		resetEnv();
// 		client.resError(500);
// 		throw;
// 	}
// }

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

// char** CGIHandler::buildEnvCStrArray() const {
// 	char** envArr = new char*[_envSize + 1];
// 	for (size_t i = 0; i < _envSize; ++i) {
// 		envArr[i] = _env[i];
// 	}
// 	envArr[_envSize] = nullptr;
// 	return envArr;
// }

std::string CGIHandler::addContentLength(const std::string& httpResponse) {
	size_t headerEndPos = httpResponse.find("\r\n\r\n");
	if (headerEndPos == std::string::npos)
		return httpResponse;
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
			// child
			if (dup2(fdResponse[1], STDOUT_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}
			if (!_requestBody.empty() && dup2(fdRequest[0], STDIN_FILENO) == -1) {
				perror("dup2");
				exit(1);
			}

			close(fdResponse[0]);
			close(fdResponse[1]);
			close(fdRequest[0]);
			close(fdRequest[1]);

			execve(_argv[0], _argv, _env.Data());
			perror("execve");
			exit(1);
		} else {
			// parent
			close(fdRequest[0]);
			if (!_requestBody.empty()) {
				write(fdRequest[1], _requestBody.c_str(), _requestBody.size());
			}
			close(fdRequest[1]);
			close(fdResponse[1]);

			struct pollfd pfd;
			pfd.fd = fdResponse[0];
			pfd.events = POLLIN;
		
			std::string responseBody;
			char buffer_[1024];
			int totalElapsed = 0;
			const int pollInterval = 25; // 25ms
			const int maxTimeout = 5000; // 5s max

			while (totalElapsed < maxTimeout) {
				int poll_ret = poll(&pfd, 1, pollInterval); // run poll every 25ms
				totalElapsed += pollInterval;

				if (poll_ret < 0) {
					kill(pid, SIGKILL);
					close(fdResponse[0]);
					waitpid(pid, 0, 0);
					throw std::runtime_error("poll() failed");
				} else if (poll_ret == 0) {
					continue; // keep polling
				}

				int bytes = read(fdResponse[0], buffer_, sizeof(buffer_));
				if (bytes < 0) {
					kill(pid, SIGKILL);
					close(fdResponse[0]);
					waitpid(pid, 0, 0);
					throw std::runtime_error("read() failed");
				} else if (bytes == 0) {
					break;
				} else {
					responseBody.append(buffer_, bytes);
				}
			}

			// timeout exceeded
			if (totalElapsed >= maxTimeout) {
				kill(pid, SIGKILL);
				close(fdResponse[0]);
				waitpid(pid, 0, 0);
				throw std::runtime_error("CGI script timeout after 5s");
			}

			close(fdResponse[0]);
			int status;
			waitpid(pid, &status, 0);

			return addContentLength(responseBody);
	}
}
