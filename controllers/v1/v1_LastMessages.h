#pragma once

#include <string>
#include <array>

#include "drogon/drogon.h"
#include "drogon/HttpController.h"

#include "glaze/glaze.hpp"

#include "fmt/core.h"

#include "utils/ResponseBuilder.hpp"
#include "models/ResponseBody.hpp"
#include "models/MessageModel.hpp"

using namespace drogon;

namespace v1
{
class LastMessages : public drogon::HttpController<LastMessages>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(LastMessages::root, "v1/lastMessages?count={1}", Get);

    METHOD_LIST_END

    static Task<> root([[maybe_unused]] HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback,
                       int count);
};
}
