/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respone.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:26:35 by chtan             #+#    #+#             */
/*   Updated: 2025/05/18 10:29:03 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

response::response()
{
}

response::~response()
{
}

response::response(const response& other)
{
    *this = other;
}

response& response::operator=(const response& other)
{
    if (this == &other)
        return *this;
    return *this;
}

void response::set_status(const std::string& status)
{
    this->_status = status;
}