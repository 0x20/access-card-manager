//
// Created by friedkiwi on 02/07/2022.
//

#include "mainview.h"

mainview::mainview(const Wt::WEnvironment& env)
        : Wt::WApplication(env) {

    // set default json filename
    JsonDataFile = "cards.json";

    setTitle("Access Card Manager");

    setTheme(std::make_shared<Wt::WBootstrap5Theme>());

    // build UI

    auto container = root() -> addWidget(std::make_unique<Wt::WContainerWidget>());

    // build base layout
    auto mainContent = container ->addWidget(std::make_unique<Wt::WContainerWidget>());
    mainContent -> addStyleClass("container");
    mainContent -> setAttributeValue("role", "main");
    mainContent -> addWidget ( std::make_unique<Wt::WText>("<br />"));
    mainContent -> addWidget (std::make_unique<Wt::WText>("<h1>Access Card Manager </h1>"));
    mainContent -> addWidget ( std::make_unique<Wt::WText>("<hr />"));

    mainContent -> addWidget (std::make_unique<Wt::WText>("This application is used to manage the access cards that can allow the space door to be opened. <br /><br />"));


    // existing card view:
    auto existingCardsView = mainContent -> addWidget(std::make_unique<Wt::WPanel>());
    existingCardsView -> setTitle("Registered cards");
    auto existingCardsContainer = existingCardsView -> setCentralWidget(std::make_unique<Wt::WContainerWidget>());

    cardsTable = existingCardsContainer ->addWidget(std::make_unique<Wt::WTable>());

    cardsTable -> addStyleClass("table");
    redrawTable();


    Wt::WPushButton *button = existingCardsContainer->addNew<Wt::WPushButton>("Add card");

    button->clicked().connect([=] {
        addCardDialog(mainContent);
    });

}

mainview::mainview(const Wt::WEnvironment& env, std::string cardsFile) : mainview(env) {
    JsonDataFile = cardsFile;
    reloadEntries();
    redrawTable();
}

void mainview::redrawTable() {
    reloadEntries();
    cardsTable -> clear();
    cardsTable -> setHeaderCount(1);
    cardsTable -> elementAt(0, 0) -> addNew<Wt::WText>("Card ID");
    cardsTable -> elementAt(0, 1) -> addNew<Wt::WText>("Owner");
    cardsTable -> elementAt(0, 2) -> addNew<Wt::WText>("Created");
    cardsTable -> elementAt(0, 3) -> addNew<Wt::WText>("Active?");
    cardsTable -> elementAt(0, 4) -> addNew<Wt::WText>("    ");

    std::list<cardentry>::iterator cardEntryIterator;

    int tableRow = 1;
    for (cardEntryIterator = cardEntries.begin(); cardEntryIterator != cardEntries.end(); cardEntryIterator++) {
        cardsTable -> elementAt(tableRow, 0) -> addNew<Wt::WText>(cardEntryIterator -> card_id);
        cardsTable -> elementAt(tableRow, 1) -> addNew<Wt::WText>(cardEntryIterator -> owner);
        cardsTable -> elementAt(tableRow, 2) -> addNew<Wt::WText>(boost::posix_time::to_simple_string(cardEntryIterator -> created));

        // add enable/disable checkbox
        auto enabledCheckbox = cardsTable -> elementAt(tableRow, 3) -> addNew<Wt::WCheckBox>("    ");
        enabledCheckbox -> setChecked(cardEntryIterator -> is_active);
        enabledCheckbox -> clicked().connect( [=] {
            cardEntryIterator -> is_active = enabledCheckbox -> isChecked();
            persistEntries();
        } );

        // add delete button
        auto deleteButton = cardsTable -> elementAt(tableRow, 4) -> addNew<Wt::WPushButton>("Delete");
        deleteButton -> clicked().connect([=] {
            auto messageBox = cardsTable -> parent() -> addChild(std::make_unique<Wt::WMessageBox>("Delete entry?",
                                                                                               "Are you sure you want to delete card '" +
                                                                                               cardEntryIterator -> card_id +
                                                                                               "' from '" +
                                                                                               cardEntryIterator -> owner +
                                                                                               "' ?",
                                                                                               Wt::Icon::Question,
                                                                                               Wt::StandardButton::Yes | Wt::StandardButton::No));
            messageBox -> buttonClicked().connect([=] {
                if (messageBox->buttonResult() == Wt::StandardButton::Yes) {
                    cardEntries.erase(cardEntryIterator);
                    persistEntries();
                    redrawTable();
                }
                cardsTable -> parent() -> removeChild(messageBox);
            });

            messageBox -> show();
        });
        tableRow ++;
    }
}

void mainview::addCardDialog(Wt::WObject *owner)
{
    auto dialog = owner -> addChild(std::make_unique<Wt::WDialog>("Add card"));

    auto dialogContents = dialog -> contents() -> addNew<Wt::WGroupBox>();


    Wt::WLabel *ownerLabel = dialogContents -> addNew<Wt::WLabel>("Owner");
    Wt::WLineEdit *ownerEdit = dialogContents -> addNew<Wt::WLineEdit>();
    ownerEdit -> setMaxLength(50);
    ownerEdit -> setWidth(250);
    ownerLabel -> setBuddy(ownerEdit);
    auto nameValidator = std::make_shared<Wt::WRegExpValidator>(".+");
    nameValidator -> setMandatory(true);
    ownerEdit -> setValidator(nameValidator);


    Wt::WLabel *cardIdLabel = dialogContents -> addNew<Wt::WLabel>("Card ID");
    Wt::WLineEdit *cardIdEdit = dialogContents -> addNew<Wt::WLineEdit>();
    cardIdLabel -> setBuddy(cardIdEdit);
    auto cardIdValidator = std::make_shared<Wt::WRegExpValidator>("[0-9A-Fa-f]+");
    cardIdValidator -> setMandatory(true);
    cardIdEdit -> setValidator(cardIdValidator);

    cardIdEdit -> enterPressed().connect( [=] {
        if (cardIdEdit -> validate() == Wt::ValidationState::Valid && ownerEdit -> validate() == Wt::ValidationState::Valid)
            dialog -> accept();
    });
    ownerEdit -> enterPressed().connect( [=] {
        if (cardIdEdit -> validate() == Wt::ValidationState::Valid && ownerEdit -> validate() == Wt::ValidationState::Valid)
            dialog -> accept();
    });


    dialog->contents()->addStyleClass("form-group");



    Wt::WPushButton *ok =dialog -> footer() -> addNew<Wt::WPushButton>("OK");

    Wt::WPushButton *cancel = dialog -> footer() -> addNew<Wt::WPushButton>("Cancel");
    dialog -> rejectWhenEscapePressed();


    ok -> clicked().connect([=] {
        if (cardIdEdit -> validate() == Wt::ValidationState::Valid && ownerEdit -> validate() == Wt::ValidationState::Valid)
            dialog -> accept();
    });

    cancel -> clicked().connect(dialog, &Wt::WDialog::reject);


    dialog -> finished().connect([=] {
        if (dialog -> result() == Wt::DialogCode::Accepted)
        {
            cardentry entry;
            entry.card_id = cardIdEdit -> text() . toUTF8();
            entry.owner = ownerEdit -> text() . toUTF8();
            entry.is_active = true;
            entry.created = boost::posix_time::second_clock::local_time();
            cardEntries.push_back(entry);

            persistEntries();
            redrawTable();
        }
        owner -> removeChild(dialog);
    });

    dialog -> show();
}

void mainview::persistEntries() {
    Json::Value jsonRoot = Json::Value(Json::arrayValue);
    std::list<cardentry>::iterator cardEntryIterator;

    for (cardEntryIterator = cardEntries.begin(); cardEntryIterator != cardEntries.end(); cardEntryIterator++) {
        jsonRoot.append(cardEntryIterator->toJsonValue());
    }

    std::ofstream outFile;
    outFile.open(JsonDataFile);
    Json::StyledWriter styledWriter;
    outFile << styledWriter.write(jsonRoot);
    outFile.close();
}

void mainview::reloadEntries() {
    Json::Value jsonRoot;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    std::ifstream inFile;

    inFile.open(JsonDataFile);
    if (Json::parseFromStream(builder, inFile, &jsonRoot, &errs)) {
        cardEntries.clear();
        for (Json::Value::ArrayIndex i = 0; i != jsonRoot.size(); i++) {
            cardEntries.push_back(cardentry(jsonRoot[i]));
        }
    } else {
        std::cout << "Error while reading JSON file:" << std::endl;
        std::cout << errs << std::endl;
    }
    inFile.close();
}