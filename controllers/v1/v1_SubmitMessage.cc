#include "v1_SubmitMessage.h"

using namespace v1;

Task<> SubmitMessage::root(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
    if (co_await ServiceDataProvider::hasReachedTheMaximumGainOfToday())
    {
        using namespace std::string_literals;
        auto response = ResponseBuilder<>()
                .setCode(HttpStatusCode::k418ImATeapot)
                .setMessage("failed: has reached to the maximum amount of today")
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
            auto response = ResponseBuilder<>()
                    .setCode(HttpStatusCode::k204NoContent)
                    .setMessage("failed: content could not be empty")
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
            const auto gain = co_await ServiceDataProvider::getNextGain();
            const auto insertedResult = co_await clientPtr->execSqlCoro(
                    "INSERT INTO rewards (id, gain, time) VALUES ($1, $2, $3)",
                    id,
                    std::to_string(gain),
                    nowTimeString
            );
            if (insertedResult.affectedRows() > 0)
            {
                LOG_DEBUG << "inserted!";
            }
        }
        else [[unlikely]]
        {
            auto response = ResponseBuilder<>()
                    .setCode(HttpStatusCode::k500InternalServerError)
                    .setMessage("failed to insert rows")
                    .build();
            callback(response);
        }
    }
    catch (const std::exception& e)
    {
        using namespace std::string_literals;
        auto response = ResponseBuilder<>()
                .setCode(HttpStatusCode::k500InternalServerError)
                .setMessage("failed: "s + e.what())
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
