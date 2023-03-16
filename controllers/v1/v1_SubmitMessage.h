#pragma once

#include <functional>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <drogon/HttpController.h>

#include "ResponseBuilder.hpp"
#include "ServiceDataProvider.hpp"

using namespace drogon;

namespace v1
{
class SubmitMessage : public drogon::HttpController<SubmitMessage>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(SubmitMessage::get, "/{2}/{1}", Get); // path is /v1/SubmitMessage/{arg2}/{arg1}
    // METHOD_ADD(SubmitMessage::your_method_name, "/{1}/{2}/list", Get); // path is /v1/SubmitMessage/{arg1}/{arg2}/list
    // ADD_METHOD_TO(SubmitMessage::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
        ADD_METHOD_TO(SubmitMessage::root, "v1/submitMessage", Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

    static Task<> root(HttpRequestPtr req,
                       std::function<void(const HttpResponsePtr&)> callback);

    static std::string getTimeString();
};
}
