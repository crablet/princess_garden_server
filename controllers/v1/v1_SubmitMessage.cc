#include "v1_SubmitMessage.h"

using namespace v1;

// Add definition of your processing function here
Task<> SubmitMessage::root(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback)
{
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

        const auto result = co_await clientPtr->execSqlCoro(
                "INSERT INTO messages (author, content, time) VALUES ($1, $2, LOCALTIMESTAMP(0))", author, content);
        if (const auto affectedRows = result.affectedRows();
                affectedRows > 0)
        {
            auto response = ResponseBuilder<decltype(affectedRows)>()
                    .setCode(HttpStatusCode::k200OK)
                    .setData(affectedRows)
                    .setMessage("success")
                    .build();
            callback(response);
        }
        else
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
