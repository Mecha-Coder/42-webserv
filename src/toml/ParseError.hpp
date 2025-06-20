/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseError.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:10:33 by rcheong           #+#    #+#             */
/*   Updated: 2025/06/20 21:09:12 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ParseError {
public:
	ParseError();
	ParseError(std::string, int);
	std::string asStr();

private:
	std::string message;
	int line;
};
