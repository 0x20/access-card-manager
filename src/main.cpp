#include <Wt/WApplication.h>
#include <string>
#include "mainview.h"

int main(int argc, char **argv)
{
    std::string cards_file = "cards.json";

    // allow setting cards.json file using environment variable CARDS_FILE
    if(const char* env_cards_file = std::getenv("CARDS_FILE"))
    {
        cards_file = std::string(env_cards_file);
    }

    return Wt::WRun(argc, argv, [&cards_file](const Wt::WEnvironment& env) {
        return std::make_unique<mainview>(env, cards_file);
    });
}