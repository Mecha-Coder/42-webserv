/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseError.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcheong <rcheong@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:33 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/04 21:18:38 by rcheong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>

class ParseError {
public:
	ParseError();
	ParseError(std::string, int);
	std::string asStr();

private:
	std::string message;
	int line;
};
