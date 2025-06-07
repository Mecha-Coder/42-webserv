/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:09:56 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/07 10:59:45 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CGIHandler.hpp"
#include <algorithm>
#include <cctype>

static std::map<std::string, std::string> ConvertHttpHeadersToCgiEnv(const std::map<std::string, std::string>& httpHeaders) {
    std::map<std::string, std::string> cgiEnv;

    for (std::map<std::string, std::string>::const_iterator it = httpHeaders.begin(); it != httpHeaders.end(); ++it) {
        const std::string& key = it->first;
        const std::string& val = it->second;

        // special case: CONTENT_TYPE and CONTENT_LENGTH
        if (key == "Content-Type") {
            cgiEnv["CONTENT_TYPE"] = val;
        } else if (key == "Content-Length") {
            cgiEnv["CONTENT_LENGTH"] = val;
        } else {
            // convert key to uppercase, replace '-' with '_', prefix with HTTP_
            std::string envKey = "HTTP_";
            for (size_t i = 0; i < key.size(); ++i) {
                char c = key[i];
                if (c == '-')
                    envKey += '_';
                else
                    envKey += std::toupper(static_cast<unsigned char>(c));
            }
            cgiEnv[envKey] = val;
        }
    }

    return (cgiEnv);
}

CGIHandler* CGIHandler::Create(const std::map<std::string, std::string>& env,
	const std::string* body,
	const std::vector<std::string>& cgiPaths) {
	CGIHandler* handler = NULL;
	try {
		std::map<std::string, std::string> cgiEnv = ConvertHttpHeadersToCgiEnv(env);

        if (cgiEnv.find("REQUEST_METHOD") == cgiEnv.end()) {
            cgiEnv["REQUEST_METHOD"] = "GET";
        }
        if (cgiEnv.find("SCRIPT_NAME") == cgiEnv.end()) {
            cgiEnv["SCRIPT_NAME"] = "";
        }
        if (cgiEnv.find("QUERY_STRING") == cgiEnv.end()) {
            cgiEnv["QUERY_STRING"] = "";
        }
		if (!body || body->empty())
			body = NULL;
		handler = new CGIHandler(env, body, cgiPaths);
		return (handler);
	} catch (...) {
		std::cerr << "CGIHandler Factory failed before construction.\n";
		delete handler;
		return NULL;
	}
}

CGIHandler::CGIHandler(const std::map<std::string, std::string>& env,
	const std::string* body,
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
}

void CGIHandler::resetArgs() {
	if (_argv) {
		if (_argv[0])
			free(_argv[0]);
		delete[] _argv;
		_argv = 0;
	}
}

std::string CGIHandler::getCmd() {
	std::map<std::string, std::string>::const_iterator it = _envMap.find("PATH_INFO");
	if (it == _envMap.end())
		throw std::runtime_error("Missing PATH_INFO in environment map");

	std::string pathInfo = it->second;
	size_t lastSlash = pathInfo.find_last_of('/');
	std::string scriptName = (lastSlash != std::string::npos) ? pathInfo.substr(lastSlash + 1) : pathInfo;

	bool allowed = false;
	for (size_t i = 0; i < _cgiPaths.size(); ++i) {
		if (_cgiPaths[i].find(scriptName) != std::string::npos) {
			allowed = true;
			break;
		}
	}
	if (!allowed)
		throw std::runtime_error("Script not allowed by configuration");

	std::string fullPath = pathInfo[0] == '/' ? "." + pathInfo : pathInfo;

	if (access(fullPath.c_str(), X_OK) != 0)
		throw std::runtime_error("Script does not exist or is not executable: " + fullPath);

	return fullPath;
}

void CGIHandler::setArgs(const std::string& cmd) {
	char* arg0 = strdup(cmd.c_str());
	if (!arg0) {
		throw std::bad_alloc();
	}
	_argv = new char*[2];
	_argv[0] = arg0;
	_argv[1] = NULL;
}

std::string CGIHandler::addContentLength(const std::string& httpResponse) {
    if (httpResponse.empty())
        return httpResponse;

    size_t headerEndPos = httpResponse.find("\r\n\r\n");
    if (headerEndPos == std::string::npos)
        return httpResponse;

    std::string headers = httpResponse.substr(0, headerEndPos);
    std::string body = httpResponse.substr(headerEndPos + 4);

    if (headers.find("Content-Length:") != std::string::npos)
        return httpResponse;

    std::istringstream headersStream(headers);
    std::vector<std::string> headerLines;
    std::string line;
    while (std::getline(headersStream, line)) {
        if (!line.empty()) {
            headerLines.push_back(line);
        }
    }

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
	if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1) {
		throw std::runtime_error("pipe() failed");
	}

	pid_t pid = fork();
	if (pid == -1) {
		throw std::runtime_error("fork() failed");
	} else if (pid == 0) {
		// child
		if (dup2(fdResponse[1], STDOUT_FILENO) == -1) {
			perror("dup2 stdout");
			exit(1);
		}
		if (_requestBody && !_requestBody->empty()) {
			if (dup2(fdRequest[0], STDIN_FILENO) == -1) {
				perror("dup2 stdin");
				exit(1);
			}
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
		std::map<std::string, std::string>::const_iterator mit = _envMap.find("REQUEST_METHOD");
		if (mit != _envMap.end()) {
			const std::string& method = mit->second;
			if ((method == "POST") && _requestBody && !_requestBody->empty()) {
				write(fdRequest[1], _requestBody->c_str(), _requestBody->size());
			}
		}

		// if (_requestBody && !_requestBody->empty()) {
		// 	ssize_t written = write(fdRequest[1], _requestBody->c_str(), _requestBody->size());
		// 	if (written < 0) {
		// 		close(fdRequest[1]);
		// 		kill(pid, SIGKILL);
		// 		waitpid(pid, 0, 0);
		// 		throw std::runtime_error("write() to CGI stdin failed");
		// 	}
		// }
		
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
			throw std::runtime_error("CGI script timeout");
		}

		close(fdResponse[0]);
		int status;
		waitpid(pid, &status, 0);

		return addContentLength(responseBody);
	}
}
