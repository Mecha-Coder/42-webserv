/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpaul <jpaul@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:25:31 by jpaul             #+#    #+#             */
/*   Updated: 2025/04/06 10:25:31 by jpaul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

// Allowed functions 
#include <sys/socket.h>  // socket, accept, listen, send, recv, bind, setsockopt
#include <netinet/in.h>  // htons
#include <sys/types.h>   // Type: pid_t, socklen_t, size_t 
#include <cstring>       // strerror
#include <cerrno>        // errno
#include <unistd.h>      // close
#include <csignal>       // signal

// Everything in C++98
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>
#include <ctime>          // localtime(), strftime()
#include "constant.hpp"
#include "prototype.hpp"

#endif
