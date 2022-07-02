//
// Created by friedkiwi on 02/07/2022.
//

#ifndef ACMGR_CARDENTRY_H
#define ACMGR_CARDENTRY_H

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <jsoncpp/json/json.h>


class cardentry {
public:
    std::string card_id;
    std::string owner;
    boost::posix_time::ptime created;
    bool is_active;

    cardentry();
    cardentry(Json::Value fromValue);
    Json::Value toJsonValue();
};


#endif //ACMGR_CARDENTRY_H
