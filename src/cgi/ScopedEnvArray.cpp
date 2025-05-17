/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopedEnvArray.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:02:25 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/17 21:08:41 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScopedEnvArray.hpp"

ScopedEnvArray::ScopedEnvArray()
	: _env(0), _envSize(0) {}

ScopedEnvArray::ScopedEnvArray(const std::map<std::string, std::string>& env) : _env(0), _envSize(0) {
	Set(env);
}

ScopedEnvArray::~ScopedEnvArray() {
	Clear();
}

ScopedEnvArray::ScopedEnvArray(const ScopedEnvArray& other) : _env(0), _envSize(0) {
    std::map<std::string, std::string> envMap;
    for (size_t i = 0; i < other._envSize; ++i) {
        std::string entry(other._env[i]);
        size_t pos = entry.find('=');
        if (pos != std::string::npos)
            envMap[entry.substr(0, pos)] = entry.substr(pos + 1);
    }
    Set(envMap);
}

ScopedEnvArray& ScopedEnvArray::operator=(const ScopedEnvArray& other) {
    if (this != &other) {
        std::map<std::string, std::string> envMap;
        for (size_t i = 0; i < other._envSize; ++i) {
            std::string entry(other._env[i]);
            size_t pos = entry.find('=');
            if (pos != std::string::npos)
                envMap[entry.substr(0, pos)] = entry.substr(pos + 1);
        }
        Set(envMap);
    }
    return *this;
}

char** ScopedEnvArray::Data() const {
	return _env;
}

size_t ScopedEnvArray::Size() const {
    return _envSize;
}

void ScopedEnvArray::Set(const std::map<std::string, std::string>& env) {
	Clear();
	_envSize = env.size();
	_env = new char*[_envSize + 1];
	size_t i = 0;
	try {
		for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it, ++i) {
			std::string kv = it->first + "=" + it->second;
			_env[i] = strdup(kv.c_str());
			if (!_env[i])
				throw std::bad_alloc();
		}
		_env[_envSize] = 0;
	} catch (...) {
		Clear();
		throw;
	}
}

void ScopedEnvArray::Clear() {
	if (_env) {
		for (size_t i = 0; i < _envSize; ++i)
			free(_env[i]);
		delete[] _env;
		_env = 0;
	}
	_envSize = 0;
}
