/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:34:01 by chtan             #+#    #+#             */
/*   Updated: 2025/04/15 22:05:35 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_HPP
#define WEB_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>      // for close()
#include <netinet/in.h>  // for sockaddr_in
#include <arpa/inet.h>   // for inet_pton(), htons()
#include <cstring>       // for memset()
#include <string>

struct http_header
{
    char* method;
    char* path;
    char* version;
    char* request_line;
    char* header;
    char* body;
    // char* host;
    // char* user_agent;
    // char* accept;
    // char* accept_encoding;
    // char* accept_language;
    // char* connection;
};

int ft_parsing(void* buffer, int buffer_len, http_header* header);

#endif