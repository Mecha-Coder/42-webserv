/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:46:15 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:54:23 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

void process_request(int sock, char *request, int end)
{
    t_request req; request[end] = '\0';
    
    if (!parse_request(req, request))
        return fail_reading(sock, 0);

    if (req.path != "/favicon.ico") 
    {
        str respond = resp_template(_200, PLAIN, 
            "Nice. Request received\n" + current_time());
        
        send(sock, respond.c_str(), respond.size(), 0);
        show_request(req);
    }
}

void fail_reading(int sock, int code)
{
    str respond = resp_template(_400, PLAIN, 
            "An error occurred during reading your request\n" + current_time());

    send(sock, respond.c_str(), respond.size(), 0);
    err_msg("Fail to read request", code);
}

void disconnected()
{std::cout << "Client disconnected\n";}
