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
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(LastMessages::get, "/{2}/{1}", Get); // path is /v1/LastMessages/{arg2}/{arg1}
        // METHOD_ADD(LastMessages::your_method_name, "/{1}/{2}/list", Get); // path is /v1/LastMessages/{arg1}/{arg2}/list
        // ADD_METHOD_TO(LastMessages::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

        ADD_METHOD_TO(LastMessages::root, "v1/lastMessages?count={1}", Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;


    static Task<> root([[maybe_unused]] HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback,
                       int count);
};
}
