/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_temp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:15:11 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 11:15:11 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

str getStatus(t_respond code)
{
    switch (code)
    {
        case _200: return "200 OK";
        case _201: return "201 Created";
        case _202: return "202 Accepted";
        case _204: return "204 No Content";
        case _301: return "301 Moved Permanently";
        case _302: return "302 Found";
        case _400: return "400 Bad Request";
        case _401: return "401 Unauthorized";
        case _403: return "403 Forbidden";
        case _404: return "404 Not Found";
        case _405: return "405 Method Not Allowed";
        case _408: return "408 Request Timeout";
        case _414: return "414 URI Too Long";
        case _429: return "429 Too Many Requests";
        case _500: return "500 Internal Server Error";
        case _501: return "501 Not Implemented";
        case _502: return "502 Bad Gateway";
        case _503: return "503 Service Unavailable";
        case _505: return "505 HTTP Version Not Supported";
    }
    return "";
}

str getContent(t_content code)
{
    switch (code)
    {
        case HTML: return "text/html";
        case PLAIN: return "text/plain";
        case JSON: return "application/json";
    }
    return "";
}

str resp_template(t_respond status, t_content content, str body)
{
    std::stringstream accum;

    accum << "HTTP/1.1 " << getStatus(status) << "\r\n"
          << "Content-Type: " << getContent(content) << "\r\n"
          << "Content-Length: " << body.size() << "\r\n"
          << "Connection: close\r\n\r\n"
          << body;
    return accum.str();
}
