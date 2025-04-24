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

str getContent(str const &filename)
{
    str acc, line;

    File INPUT(filename.c_str());
    if (INPUT.is_open())
    {
        while(std::getline(INPUT, line))
            acc += line;
    }
    return acc;
}

void process_request(int sock, char *request, int end)
{
    t_request req; request[end] = '\0';
    str respond, body;

    if (!parse_request(req, request))
        return fail_reading(sock, 0);

    if (req.path != "/favicon.ico") 
    {
        if (req.path == "/upload" && req.method == "POST")
        {
            char append[BUFFER_SIZE + 1];
            int bytes;

            size_t len = atoi(req.header["Content-Length"].c_str());
            while (len > req.body.size())
            {
                bytes = recv(sock, append, BUFFER_SIZE, 0);
                append[bytes] = '\0';
                req.body += append;
            }
                
            
            body = "{"
            "\"message\": \"2 file(s) uploaded successfully\","
            "\"files\": [\"file1.txt\",\"image.jpg\"]"
            "}";

            respond = resp_template(_200, JSON, body);
            send(sock, respond.c_str(), respond.size(), 0);
        }
        else
        {
            body = getContent("./page/" + req.path.substr(1) + ".html");
    
            if (body.empty()) {
                body = getContent("./page/404.html");
                respond = resp_template(_404, HTML, body); 
            }
            else
                respond = resp_template(_200, HTML, body); 
            
            send(sock, respond.c_str(), respond.size(), 0);
        }
       
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
