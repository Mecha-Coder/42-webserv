/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:09:50 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/20 21:21:12 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <cstdio>
#include <signal.h>
#include "ScopedEnvArray.hpp"

// class CGIHandler {
// 	public:
// 		static CGIHandler* Create(const std::map<std::string, std::string>& env,
// 			const std::string& body,
// 			const std::vector<std::string>& cgiPaths);
// 		CGIHandler(const std::map<std::string, std::string>& env, const std::string& body, const std::vector<std::string>& cgiPaths);
// 		~CGIHandler();
	
// 		void setEnv(const std::map<std::string, std::string>& env);
// 		void cleanEnv();
// 		std::string getFileExtension(const std::string& filename);
// 		std::string getCmd();
// 		void setArgs(const std::string& cmd);
// 		char** buildEnvCStrArray() const;
// 		std::string addContentLength(const std::string& httpResponse);
// 		std::string Execute();
// 	private:
// 		std::map<std::string, std::string> _envMap;
// 		std::vector<std::string> _cgiPaths;
// 		std::string _requestBody;
// 		char** _env;
// 		size_t _envSize;
// 		char** _argv;
// 		std::string _cgiPath;
// };

class CGIHandler {
	public:
		static CGIHandler* Create(
			const std::map<std::string, std::string>& env,
			const std::string* body,
			const std::vector<std::string>& cgiPaths);
	
		~CGIHandler();
	
		std::string Execute();
	
	// private:
		CGIHandler(const std::map<std::string, std::string>& env,
				   const std::string* body,
				   const std::vector<std::string>& cgiPaths);
	
		// void setEnv(const std::map<std::string, std::string>& env);
		// void resetEnv();
		void resetArgs();
	
		std::string getCmd();
		void setArgs(const std::string& cmd);
		std::string addContentLength(const std::string& httpResponse);
		// char** buildEnvCStrArray() const;
	
	private:
		std::map<std::string, std::string> _envMap;
		std::vector<std::string> _cgiPaths;
		const std::string* _requestBody;
		std::string _cgiPath;
	
		ScopedEnvArray _env;
		ScopedEnvArray _envSize;
		char** _argv;
	};
