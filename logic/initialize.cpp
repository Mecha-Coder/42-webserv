/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:56:44 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/06 10:56:44 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

int create_socket()
{return socket(AF_INET, SOCK_STREAM, 0);}

int setup_socket(int reuse = 1)
{return setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));}

int bind_to_port()
{
    struct sockaddr_in addr;
    struct sockaddr *server_addr;
    
    memset(&addr.sin_zero, 0, 8);        // Zero out padding
    addr.sin_family      = AF_INET;      // Select IPV4
    addr.sin_addr.s_addr = INADDR_ANY;   // Listen on any IP
    addr.sin_port        = htons(8080);  // Port 8080 convert to network byte
    
    server_addr =  reinterpret_cast<struct sockaddr *>(&addr);
    return bind(server_sock, server_addr, sizeof(addr));
}

bool initialize()
{   
    server_sock = create_socket();
    if (server_sock == -1)             return (err_msg("create_socket", errno));
    if (setup_socket() == -1)          return (err_msg("setup_socket", errno));
    if (bind_to_port() == -1)          return (err_msg("bind_to_port", errno));
    if (listen(server_sock, 5) == -1)  return (err_msg("set_to_listen", errno));
    
    return true;
}
