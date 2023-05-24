#pragma once

#include <string>
#include <functional>

#include <drogon/HttpController.h>

#include "fmt/core.h"

#include "ResponseBuilder.hpp"

using namespace drogon;

namespace v1
{
class RemainingLifespan : public drogon::HttpController<RemainingLifespan>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(RemainingLifespan::root, "v1/remainingLifespan", Get);
    METHOD_LIST_END

    static Task<> root([[maybe_unused]] HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback);
};
}
