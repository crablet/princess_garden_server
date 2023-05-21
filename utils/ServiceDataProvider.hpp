//
// Created by crablet on 2023/3/17.
//

#ifndef PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H
#define PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H

#include <random>

#include "drogon/drogon.h"

using namespace drogon;

class ServiceDataProvider
{
public:
    static Task<int> getGainTimeOfToday()
    {
        auto clientPtr = app().getDbClient();
        const auto result = co_await clientPtr->execSqlCoro(
                "SELECT SUM(gain) as total FROM rewards WHERE current_date = date(time)");
        const auto gainTimeOfToday = result.front()["total"].as<int>();

        co_return gainTimeOfToday;
    }

    static constexpr int MAX_GAIN_OF_A_DAY = 1999;

    static Task<bool> hasReachedTheMaximumGainOfToday()
    {
        const auto currentGain = co_await getGainTimeOfToday();

        co_return currentGain >= MAX_GAIN_OF_A_DAY;
    }

    static Task<int> getNextGain()
    {
        if (co_await hasReachedTheMaximumGainOfToday())
        {
            co_return 0;
        }
        else
        {
            const auto currentGain = co_await getGainTimeOfToday();
            const auto maxLeftGain = MAX_GAIN_OF_A_DAY - currentGain;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, maxLeftGain);
            const auto nextGain = dis(gen);

            co_return nextGain;
        }
    }
};

#endif //PRINCESS_GARDEN_SERVER_SERVICEDATAPROVIDER_H
