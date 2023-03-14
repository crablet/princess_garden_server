#include "v1_LastMessages.h"

using namespace v1;

Task<>
LastMessages::root([[maybe_unused]] HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, int count)
{
    auto clientPtr = app().getDbClient();
    try
    {
        const auto actualCount = std::min(count, 53);
        const auto result = co_await clientPtr->execSqlCoro("SELECT * FROM messages ORDER BY id DESC LIMIT $1",
                                                            std::to_string(actualCount));
        const auto resultSize = result.size();
        if (0 < resultSize && resultSize <= count) [[likely]]
        {
            auto resultVector = std::vector<MessageModel>{};
            for (const auto& row : result)
            {
                auto currentMessage = MessageModel{
                        .id = row["id"].as<int>(),
                        .author = (row["author"].isNull() ? "null" : row["author"].as<std::string>()),
                        .content = row["content"].as<std::string>(),
                        .time = row["content"].as<std::string>()
                };
                resultVector.push_back(currentMessage);
            }

            const auto message = count <= actualCount ? "success" : "success. note: the maximum count is 53";

            const auto response = ResponseBuilder<decltype(resultVector)>()
                    .setCode(HttpStatusCode::k200OK)
                    .setData(resultVector)
                    .setMessage(message)
                    .build();
            callback(response);
        }
        else if (resultSize <= 0) [[unlikely]]
        {
            const auto response = ResponseBuilder<std::string>()
                    .setCode(HttpStatusCode::k500InternalServerError)
                    .setData("")
                    .setMessage("unknown error")
                    .build();
            callback(response);
        }
        else
        {
            const auto response = ResponseBuilder<std::string>()
                    .setCode(HttpStatusCode::k500InternalServerError)
                    .setData("")
                    .setMessage("index out of range")
                    .build();
            callback(response);
        }
    }
    catch (const orm::DrogonDbException& e)
    {
        auto response = ResponseBuilder<std::string>()
                .setCode(HttpStatusCode::k500InternalServerError)
                .setData(e.base().what())
                .setMessage("exception occurred")
                .build();
        callback(response);
    }

    co_return;
}
