/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseError.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:35 by rcheong           #+#    #+#             */
/*   Updated: 2025/05/08 21:10:36 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseError.hpp"

template <typename T> std::string toStr(T value);

ParseError::ParseError() { }

std::string ParseError::asStr() {
		return "ParseError: [" + message + "] at line " + toStr(line);
	}

ParseError::ParseError(std::string message, int line) {
	this->message = message;
	this->line = line;
}
