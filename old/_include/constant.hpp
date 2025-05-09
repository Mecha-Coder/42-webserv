/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constant.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:57:06 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:04:58 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#define BUFFER_SIZE 4096

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[33m"
#define RESET  "\033[0m"

extern int server_sock;  // Make it global, so it can be closed using signal 
typedef std::string str;
typedef std::map<str, str>::iterator map_It;
typedef std::ifstream File;

typedef enum response_status
{
    _200, _201, _202, _204, _301, _302,
    _400, _401, _403, _404, _405, _408, 
    _414, _429, _500, _501, _502, _503, 
    _505,
} t_respond;


typedef enum response_content_type
{
    HTML, PLAIN, JSON,
} t_content;


typedef struct s_request
{
    str method;
    str path;
    str protocol;
    std::map<str, str> header;
    std::string body;
} t_request;

#endif 