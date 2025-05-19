/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   respond_check.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chtan <chtan@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:46:36 by chtan             #+#    #+#             */
/*   Updated: 2025/05/19 15:11:37 by chtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include "client_tmp.hpp"

// header::header()
// {
// }

// header::~header()
// {
// }

// header::header(const header& other)
// {
//     *this = other;
// }

// header& header::operator=(const header& other)
// {
//     if (this == &other)
//         return *this;
//     return *this;
// }

// std::string header::err_msg(int i)
// {
//     std::string result;
//     std::map<int, std::pair<std::string, std::string>>::iterator error_num = errorDescriptions.find(i);

//     if (error_num != errorDescriptions.end() && i = 1) {
//         result = error_num->second.first;
//     }
//     else if (error_num != errorDescriptions.end() && i = 2) {
//         result = error_num->second.second;
//     }
//     else {
//         result = "Unknown error code.";
//     }
//     return result;   
// }

// Define the errorDescriptions map
// std::map<int, std::pair<std::string, std::string>> errorDescriptions = {
//     {404, {"Not Found", "The requested resource could not be found"}},
//     {500, {"Internal Server Error", "An unexpected condition was encountered"}}
// };

// error_code enum is error codes and i is error message in short or long
// std::string header::get_map(int error_code) const
// {
//     std::string result;
//     std::map<int, std::pair<std::string, std::string>>::iterator error_num = errorDescriptions.find(error_code);

//     auto err = errorDescriptions.find(ERROR_403);
//     if (err != errorDescriptions.end()) {
//         result = err->second.first + ": " + err->second.second;
//     } else {
//         result = "Unknown error code.";
//     }
//     return result;
// }

// std::string get_map(error_code, int i)
// {
//     std::string result;
//     // auto err = errorDescriptions.find(404);
//     if (err != errorDescriptions.end()) {
//         result = err->second.first + ": " + err->second.second;
//     } else {
//         result = "Unknown error code.";
//     }
//     return result;
// }
void testing(){
    std::vector<std::string> files = read_dir("./website");
}

int main(void)
{
    // header h;//construct
    Client c;//construct
    response r;//construct
    bool check = false;

    // listDirectoryPOSIX(c.get_path(1));//get main path and check
    // checkPathAndSetResponse("404.html", r);
    // std::cout << search("./website", "404.html", check);
    // if (check) {
    //     std::cout << "File found!" << std::endl;
    // } else {
    //     search_recursive("./website", "404.html");
    //     std::cout << "File not found." << std::endl;
    // }
    
    // std::cout << r.get_status() << std::endl;
    
    return 0;
}
