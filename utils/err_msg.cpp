/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:27:52 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:10:54 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_include/server.hpp"

bool err_msg(str const &msg, int code)
{
    str Msg = " => " + msg;
    str Code = " => " + std::string(strerror(code));

    std::cerr << RED "Error "  RESET
         << (msg.empty()?   "" : Msg)
         << (code?        Code : "") 
         << "\n";
    return false;
}

/*
int main()
{
    err_msg("", 5);
    err_msg("Hello");
    err_msg("Hello", 5);
}
*/
