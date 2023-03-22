//
// Created by crablet on 2023/3/11.
//

#ifndef PRINCESS_GARDEN_SERVER_RESPONSEBUILDER_HPP
#define PRINCESS_GARDEN_SERVER_RESPONSEBUILDER_HPP

#include <string>
#include <type_traits>

#include "drogon/drogon.h"

#include "ResponseBody.hpp"

using namespace drogon;

template <typename T = void>
class ResponseBuilder
{
    using BodyType = std::remove_cvref_t<T>;

public:
    ResponseBuilder() = default;

    ResponseBuilder& setCode(HttpStatusCode statusCode)
    {
        this->code = statusCode;

        return *this;
    }

    ResponseBuilder& setData(const T& json)
    {
        this->responseJson = json;

        return *this;
    }

    ResponseBuilder& setMessage(const std::string& messageString)
    {
        this->message = messageString;

        return *this;
    }

    HttpResponsePtr build()
    {
        auto body = ResponseBody<BodyType>{
                .code = code,
                .data = responseJson,
                .message = message,
        };
        auto bodyString = glz::write_json(body);

        auto response = HttpResponse::newHttpResponse();
        response->setContentTypeString("application/json");
        response->setStatusCode(HttpStatusCode::k200OK);    // 能回应的其实都是200OK，具体错误在body里再展示
        response->setBody(bodyString);

        // todo: 这里需要允许跨域吗？
        // response->addHeader("Access-Control-Allow-Credentials", "true");
        // response->addHeader("Access-Control-Allow-Origin", "*");

        return response;
    }

private:
    HttpStatusCode code{};
    BodyType responseJson;
    std::string message;
};

template <>
class ResponseBuilder<void>
{
public:
    ResponseBuilder() = default;

    ResponseBuilder& setCode(HttpStatusCode statusCode)
    {
        this->code = statusCode;

        return *this;
    }

    ResponseBuilder& setMessage(const std::string& messageString)
    {
        this->message = messageString;

        return *this;
    }

    HttpResponsePtr build()
    {
        auto body = ResponseBody<>{
                .code = code,
                .message = message,
        };
        auto bodyString = glz::write_json(body);

        auto response = HttpResponse::newHttpResponse();
        response->setContentTypeString("application/json");
        response->setStatusCode(HttpStatusCode::k200OK);    // 能回应的其实都是200OK，具体错误在body里再展示
        response->setBody(bodyString);

        // todo: 这里需要允许跨域吗？
        // response->addHeader("Access-Control-Allow-Credentials", "true");
        // response->addHeader("Access-Control-Allow-Origin", "*");

        return response;
    }

private:
    HttpStatusCode code{};
    std::string message;
};

#endif //PRINCESS_GARDEN_SERVER_RESPONSEBUILDER_HPP
