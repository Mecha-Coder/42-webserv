/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopedEnvArray.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:13:13 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/24 15:57:07 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

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
