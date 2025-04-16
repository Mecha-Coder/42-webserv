/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _main.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:48:03 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:08:27 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

int server_sock;

void close_server(int sig)
{
    (void)sig;
    close(server_sock);
    std::cout << YELLOW "Shutting down server!\n" RESET;
    exit(EXIT_SUCCESS);
}

int main()
{
    if (!initialize())
        return (close(server_sock), EXIT_FAILURE);
    
    signal(SIGINT, close_server);
    std::cout << "Server is listening on => http://localhost:8080/\n";
    while (true) 
        listen_request();
}
