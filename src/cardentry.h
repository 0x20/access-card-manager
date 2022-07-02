//
// Created by friedkiwi on 02/07/2022.
//

#ifndef ACMGR_CARDENTRY_H
#define ACMGR_CARDENTRY_H

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>


class cardentry {
public:
    std::string card_id;
    std::string owner;
    boost::posix_time::ptime created;
    bool is_active;

    cardentry();
};


#endif //ACMGR_CARDENTRY_H
