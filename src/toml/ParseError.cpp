/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseError.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:35 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/20 21:10:04 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseError.hpp"
template <typename T> std::string toStr(T value);

std::string ParseError::asStr() {
	return "ParseError: [" + message + "] at line " + toStr(line);
}

ParseError::ParseError() { }

ParseError::ParseError(std::string message, int line) {
	this->message = message;
	this->line = line;
}
