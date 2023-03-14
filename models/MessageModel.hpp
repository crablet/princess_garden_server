//
// Created by crablet on 2023/3/11.
//

#ifndef PRINCESS_GARDEN_SERVER_LASTMESSAGESMODEL_H
#define PRINCESS_GARDEN_SERVER_LASTMESSAGESMODEL_H

#include <string>

#include "glaze/glaze.hpp"
#include "glaze/core/macros.hpp"

struct MessageModel
{
    int id;
    std::string author;
    std::string content;
    std::string time;
};

GLZ_META(MessageModel, id, author, content, time);

#endif //PRINCESS_GARDEN_SERVER_LASTMESSAGESMODEL_H
