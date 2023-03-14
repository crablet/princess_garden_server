//
// Created by crablet on 2023/3/11.
//

#ifndef PRINCESS_GARDEN_SERVER_RESPONSEBODY_HPP
#define PRINCESS_GARDEN_SERVER_RESPONSEBODY_HPP

#include <string>

#include "glaze/glaze.hpp"
#include "glaze/core/macros.hpp"

template <typename DataType>
struct ResponseBody
{
    int code{};
    DataType data;
    std::string message;

    GLZ_LOCAL_META(ResponseBody, code, data, message);
};

#endif //PRINCESS_GARDEN_SERVER_RESPONSEBODY_HPP
