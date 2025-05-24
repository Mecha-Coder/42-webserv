/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:50:37 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/16 14:44:45 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_HPP
#define PROTOTYPE_HPP

#include "constant.hpp"

///////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////

bool initialize();




///////////////////////////////////////////////////////////
// REQUEST
///////////////////////////////////////////////////////////

bool listen_request();
bool parse_request(t_request &req, str s);


///////////////////////////////////////////////////////////
// REQUEST
///////////////////////////////////////////////////////////

void process_request(int sock, char *reqest, size_t end);
void fail_reading(int sock, int code);
void disconnected();


///////////////////////////////////////////////////////////
// UTILS
///////////////////////////////////////////////////////////

bool err_msg(str const &msg, int code = 0);
str resp_template(t_respond status, t_content content = PLAIN, str body = "");
void show_request(t_request &req);
str current_time();

#endif