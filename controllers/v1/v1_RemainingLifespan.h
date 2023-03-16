#pragma once

#include <functional>

#include <drogon/HttpController.h>

#include "ResponseBuilder.hpp"

using namespace drogon;

namespace v1
{
class RemainingLifespan : public drogon::HttpController<RemainingLifespan>
{
public:
    METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(RemainingLifespan::get, "/{2}/{1}", Get); // path is /v1/RemainingLifespan/{arg2}/{arg1}
        // METHOD_ADD(RemainingLifespan::your_method_name, "/{1}/{2}/list", Get); // path is /v1/RemainingLifespan/{arg1}/{arg2}/list
        // ADD_METHOD_TO(RemainingLifespan::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(RemainingLifespan::root, "v1/remainingLifespan", Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

    static Task<> root([[maybe_unused]] HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback);
};
}
