//
// Created by friedkiwi on 02/07/2022.
//

#include "cardentry.h"

cardentry::cardentry() {

}

cardentry::cardentry(Json::Value fromValue) : cardentry() {
    card_id = fromValue["card_id"].asString();
    owner = fromValue["owner"].asString();
    is_active = fromValue["is_active"].asBool();
    created = boost::posix_time::from_time_t(fromValue["created"].asInt64());
}

Json::Value cardentry::toJsonValue() {
    Json::Value cardEntryValue;
    const boost::posix_time::ptime epoch = boost::posix_time::from_time_t(0);
    boost::posix_time::time_duration created_unix_time = created - epoch;

    cardEntryValue["card_id"] = card_id;
    cardEntryValue["owner"] = owner;
    cardEntryValue["is_active"] = is_active;
    cardEntryValue["created"] = (Json::Int64) created_unix_time.total_seconds();

    return cardEntryValue;
}
