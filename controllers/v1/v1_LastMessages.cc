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
                        .time = row["time"].as<std::string>()
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
            using namespace std::string_literals;
            const auto response = ResponseBuilder<>()
                    .setCode(HttpStatusCode::k500InternalServerError)
                    .setMessage("unknown error")
                    .build();
            callback(response);
        }
        else
        {
            const auto response = ResponseBuilder<>()
                    .setCode(HttpStatusCode::k500InternalServerError)
                    .setMessage("index out of range")
                    .build();
            callback(response);
        }
    }
    catch (const orm::DrogonDbException& e)
    {
        using namespace std::string_literals;
        auto response = ResponseBuilder<>()
                .setCode(HttpStatusCode::k500InternalServerError)
                .setMessage("exception occurred"s + e.base().what())
                .build();
        callback(response);
    }

    co_return;
}
