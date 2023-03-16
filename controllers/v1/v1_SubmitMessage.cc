#include "v1_SubmitMessage.h"

using namespace v1;

Task<> SubmitMessage::root(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
    if (co_await ServiceDataProvider::hasReachedTheMaximumGainOfToday())
    {
        auto response = ResponseBuilder<std::string>()
                .setCode(HttpStatusCode::k418ImATeapot)
                .setData("has reached to the maximum amount of today")
                .setMessage("failed")
                .build();
        callback(response);

        co_return;
    }

    const auto clientPtr = app().getDbClient();
    try
    {
        auto json = req->getParameters();
        const auto author = json["author"];
        const auto content = json["content"];

        if (content.empty())
        {
            auto response = ResponseBuilder<std::string>()
                    .setCode(HttpStatusCode::k200OK)
                    .setData("content could not be empty")
                    .setMessage("failed")
                    .build();
            callback(response);

            co_return;
        }

        const auto nowTimeString = getTimeString();
        const auto result = co_await clientPtr->execSqlCoro(
                "INSERT INTO messages (author, content, time) VALUES ($1, $2, LOCALTIMESTAMP(0)) RETURNING id",
                author,
                content);
        if (const auto affectedRows = result.affectedRows();
                affectedRows > 0) [[likely]]
        {
            auto response = ResponseBuilder<decltype(affectedRows)>()
                    .setCode(HttpStatusCode::k200OK)
                    .setData(affectedRows)
                    .setMessage("success")
                    .build();
            callback(response);

            const auto id = result.front()["id"].as<std::string>();
            const auto insertedResult = co_await clientPtr->execSqlCoro(
                    "INSERT INTO rewards (id, gain, time) VALUES ($1, $2, $3)",
                    id,
                    "53",   // todo: 这里需要计算出正确的数值
                    nowTimeString
            );
            if (insertedResult.affectedRows() > 0)
            {
                LOG_DEBUG << "inserted!";
            }
        }
        else [[unlikely]]
        {
            auto response = ResponseBuilder<decltype(affectedRows)>()
                    .setCode(HttpStatusCode::k200OK)
                    .setData(affectedRows)
                    .setMessage("failed")
                    .build();
            callback(response);
        }
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

std::string SubmitMessage::getTimeString()
{
    const auto now = std::chrono::system_clock::now();
    const auto nowTimeT = std::chrono::system_clock::to_time_t(now);
    const auto tm = *std::localtime(&nowTimeT);
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}
