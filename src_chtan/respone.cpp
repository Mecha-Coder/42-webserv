/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respone.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:26:35 by chtan             #+#    #+#             */
/*   Updated: 2025/05/19 12:00:35 by chtan            ###   ########.fr       */
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

void response::set_content(const int content)
{
    this->_content = content;
}

void response::set_content_length(const std::string& len) {
    this->_content_length = len;
}

std::string response::get_status() const {
    if (_status.empty()) {
        return "No status set";
    }
    return _status;
}

int response::get_content_type() const {
    return _content_type;
}

std::string response::get_content_length() const {
    return _content_length;
}
