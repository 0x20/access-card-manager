//
// Created by friedkiwi on 02/07/2022.
//

#ifndef ACMGR_MAINVIEW_H
#define ACMGR_MAINVIEW_H


#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBootstrap5Theme.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WAnimation.h>
#include <Wt/WPanel.h>
#include <Wt/WDialog.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WLabel.h>
#include <Wt/WEnvironment.h>
#include <Wt/WTable.h>
#include <Wt/WText.h>
#include <Wt/WGroupBox.h>
#include <Wt/WHBoxLayout.h>

#include <list>
#include "cardentry.h"

class mainview : public Wt::WApplication {
public:
    mainview(const Wt::WEnvironment& env);

private:
    std::list<cardentry> cardEntries;
    Wt::WTable * cardsTable;

    void addCardDialog(Wt::WObject *owner);
};


#endif //ACMGR_MAINVIEW_H
