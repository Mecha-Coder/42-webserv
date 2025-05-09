/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:45:21 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:39:34 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

void show_request(t_request &req)
{
    std::cout << YELLOW << "===============================================" << RESET << "\n\n"
    << GREEN "-- REQUEST LINE ------------------------------\n" RESET          
    << "Method     : " << req.method    << "\n"
    << "Path       : " << req.path      << "\n"
    << "Protocol   : " << req.protocol  << "\n\n"

    << GREEN "-- HEADER ------------------------------------\n" RESET;
    for (map_It i = req.header.begin(); i != req.header.end(); i++)
        std::cout << i->first << " => " << i->second << "\n";
    
    
    std::cout 
    << GREEN "\n-- BODY ------------------------------------------\n" RESET
    << req.body << "\n"
    << YELLOW << "===============================================" << RESET << "\n";
}