/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseError.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:33 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/08 21:10:34 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ParseError {
public:
	ParseError();
	ParseError(std::string, int);

	std::string as_str() {
		return "ParseError: [" + message + "] at line " + std::to_string(line);
	}

private:
	std::string message;
	int line;
};
