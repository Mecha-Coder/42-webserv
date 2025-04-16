/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 09:16:59 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 09:16:59 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

int waiting()
{
    struct sockaddr_in addr;
    struct sockaddr *client_addr;
    
    socklen_t len = sizeof(addr);
    client_addr = reinterpret_cast<struct sockaddr *>(&addr);
    return accept(server_sock, client_addr, &len);
}

bool listen_request()
{   
    int new_sock, read_bytes;
    char request[BUFFER_SIZE];
   
    new_sock = waiting();
    if (new_sock == -1) 
        return (err_msg("connection failed", errno));

    read_bytes = recv(new_sock, request, BUFFER_SIZE, 0);
    if      (read_bytes > 0)  process_request(new_sock, request, read_bytes);
    else if (read_bytes < 0)  fail_reading(new_sock, errno);
    else                      disconnected();

    return (close(new_sock), true);
}
