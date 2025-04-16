/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:05:09 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:07:27 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

bool parse_line(t_request &req, str &s)
{
    size_t space1, space2, end;

    end = s.find("\r\n");
    if (end == s.npos) return false;

    space1 = s.find(' ');
    if (space1 > end) return false;
    
    space2 = s.find(' ', space1 + 1);
    if (space2 > end) return false;

    req.method   = s.substr(0, space1);
    req.path     = s.substr(space1 + 1, space2 - space1 - 1);
    req.protocol = s.substr(space2 + 1, end   - space2 - 1);
    s.erase(0, end + 2);
    
    return true;
}

bool parse_header(t_request &req, str &s)
{
    size_t colon, end;
    str header, key, value;

    end = s.find("\r\n\r\n");
    if (end == s.npos) return false;
    
    header = s.substr(0, end + 2);
    s.erase(0, end + 4);

    while (end = header.find("\r\n"), end != header.npos)
    {
        colon = header.find(": ");
        if (colon > end) return false;
        
        key = header.substr(0, colon);
        value = header.substr(colon + 2, end - colon - 2); 
        req.header[key] = value;
        header.erase(0, end + 2); 
    }
    return true;
}

bool validate_line(t_request &req)
{
    if (req.method != "GET" && req.method != "POST" && req.method != "DELETE")
        return false;
    if (req.protocol != "HTTP/1.1")
        return false;
    return true;
}

bool parse_request(t_request &req, str s)
{
    if (!parse_line(req, s) || !validate_line(req) || !parse_header(req, s))
        return false;
    req.body = s;
    return true;
}
