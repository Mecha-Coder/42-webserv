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

    File INPUT(filename.c_str(), std::ios::binary);
    if (INPUT.is_open())
    {
        while(std::getline(INPUT, line))
            acc += line;
    }
    return acc;
}

void process_request(int sock, char *request, size_t end)
{
    t_request req;
    str respond, body;

    str storeBytes; storeBytes.append(request, end);
    if (!parse_request(req, storeBytes))
        return fail_reading(sock, 0);

    if (req.method == "POST")
    { 
        body = "{"
        "\"message\": \"2 file(s) uploaded successfully\","
        "\"files\": [\"file1.txt\",\"image.jpg\"]"
        "}";

        respond = resp_template(_200, JSON, body);
        send(sock, respond.c_str(), respond.size(), 0);
    }
    else
    {
        if (req.path == "/")
            body = getContent("./website/main.html");
        else
            body = getContent("./website" + req.path);
    
        if (body.empty()) {
            body = getContent("./website/404.html");
            respond = resp_template(_404, HTML, body); 
        }
        else respond = resp_template(_200, HTML, body); 
        send(sock, respond.c_str(), respond.size(), 0);
    }
    
    std::cout << "Size is = " << req.body.size() << "\n\n"
              << req.body << std::endl;
    
    for (size_t i = 0; i < storeBytes.size(); i++)
    {
        switch (storeBytes[i])
        {
            case 0: std::cout << "\\0"; break;
            case 1: std::cout << "\\x01"; break;
            case 2: std::cout << "\\x02"; break;
            case 3: std::cout << "\\x03"; break;
            case 4: std::cout << "\\x04"; break;
            case 5: std::cout << "\\x05"; break;
            case 6: std::cout << "\\x06"; break;
            case 7: std::cout << "\\a"; break;
            case 8: std::cout << "\\b"; break;
            case 9: std::cout << "\\t"; break;
            case 10: std::cout << "\\n\n"; break;
            case 11: std::cout << "\\v"; break;
            case 12: std::cout << "\\f"; break;
            case 13: std::cout << "\\r"; break;
            case 14: std::cout << "\\x0E"; break;
            case 15: std::cout << "\\x0F"; break;
            case 16: std::cout << "\\x10"; break;
            case 17: std::cout << "\\x11"; break;
            case 18: std::cout << "\\x12"; break;
            case 19: std::cout << "\\x13"; break;
            case 20: std::cout << "\\x14"; break;
            case 21: std::cout << "\\x15"; break;
            case 22: std::cout << "\\x16"; break;
            case 23: std::cout << "\\x17"; break;
            case 24: std::cout << "\\x18"; break;
            case 25: std::cout << "\\x19"; break;
            case 26: std::cout << "\\x1A"; break;
            case 27: std::cout << "\\x1B"; break;
            case 28: std::cout << "\\x1C"; break;
            case 29: std::cout << "\\x1D"; break;
            case 30: std::cout << "\\x1E"; break;
            case 31: std::cout << "\\x1F"; break;
            case 127: std::cout << "\\x7F"; break;
            case 63 : std::cout << "\\?"; break;
            case 34 : std::cout << "\\\""; break;
            case 39 : std::cout << "\\'"; break;
            case 92 : std::cout << "\\\\"; break;
            default: std::cout << storeBytes[i];
        }                          
    }
    std::cout << "------------------------------\n\n";
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
