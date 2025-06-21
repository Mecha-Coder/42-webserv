#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

class ScopedEnvArray {
	public:
		ScopedEnvArray();
		explicit ScopedEnvArray(const std::map<std::string, std::string>& env);
		~ScopedEnvArray();
		ScopedEnvArray(const ScopedEnvArray& other);
    	ScopedEnvArray& operator=(const ScopedEnvArray& other);
	
		void Set(const std::map<std::string, std::string>& env);
		char** Data() const;
		size_t Size() const;
		void Clear();
	
	private:
		char** _env;
		size_t _envSize;
	};
