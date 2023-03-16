#include "v1_RemainingLifespan.h"

using namespace v1;

Task<>
RemainingLifespan::root([[maybe_unused]] HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
    auto clientPtr = app().getDbClient();
    try
    {
        const auto result = co_await clientPtr->execSqlCoro("SELECT SUM(gain) as total FROM rewards");
        const auto total = result.front()["total"].as<int>();
        auto response = ResponseBuilder<decltype(total)>()
                .setCode(HttpStatusCode::k200OK)
                .setData(total)
                .setMessage("success")
                .build();
        callback(response);
    }
    catch (const std::exception& e)
    {
        auto response = ResponseBuilder<std::string>()
                .setCode(HttpStatusCode::k500InternalServerError)
                .setData(e.what())
                .setMessage("failed")
                .build();
        callback(response);
    }

    co_return;
}
