//
// Created by crablet on 2023/3/17.
//

#ifndef PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H
#define PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H

#include "drogon/drogon.h"

using namespace drogon;

class ServiceDataProvider
{
public:
    static Task<int> getGainTimeOfToday()
    {
        auto clientPtr = app().getDbClient();
        const auto result = co_await clientPtr->execSqlCoro(
                "SELECT SUM(gain) as total FROM rewards WHERE current_date = time");

        co_return result.front()["total"].as<int>();
    }

    static constexpr int MAX_GAIN_OF_A_DAY = 1999;

    static Task<bool> hasReachedTheMaximumGainOfToday()
    {
        const auto currentGain = co_await getGainTimeOfToday();

        co_return currentGain >= MAX_GAIN_OF_A_DAY;
    }
};

#endif //PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H
