/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resErr.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:10:27 by chtan             #+#    #+#             */
/*   Updated: 2025/05/18 15:11:02 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include "client_tmp.hpp"

void    respond_error(int error)
{
    ;
}

void    respond_default(response &res)
{
    //check condition
    str tmp = "/website/default";
    str result = str("./home") + tmp + ".html";
}
