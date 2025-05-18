/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resDirlist.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:07:42 by chtan             #+#    #+#             */
/*   Updated: 2025/05/18 14:31:26 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"

void    resDirlist(response &res)
{
    str main_path = "./website";
    // str res = main_path + "404.error" + ".html";
    str tmp = listDirectoryPOSIX("404.html");
    str res = main_path + tmp;
    std::cout << "resDirlist function done!!!" << std::endl;
}