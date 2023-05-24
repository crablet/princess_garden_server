#pragma once

#include <string>
#include <functional>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <drogon/HttpController.h>

#include "fmt/core.h"

#include "ResponseBuilder.hpp"
#include "ServiceDataProvider.hpp"

using namespace drogon;

namespace v1
{
class SubmitMessage : public drogon::HttpController<SubmitMessage>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(SubmitMessage::root, "v1/submitMessage", Post);
    METHOD_LIST_END

    static Task<> root(HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback);

    static std::string getTimeString();
};
}
