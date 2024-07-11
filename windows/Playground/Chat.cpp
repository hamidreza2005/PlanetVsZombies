#include "Chat.h"
#include "../../core/Cookie.h"

QMap<QString,QString> Chat::allEmoji = {
        {"neutral",":/resources/images/emoji/NeutralFaceEmoji.png"},
        {"poop",":/resources/images/emoji/PoopEmoji.png"},
        {"laugh",":/resources/images/emoji/TearsofJoyEmoji.png"},
        {"love",":/resources/images/emoji/HeartEyesEmoji.png"},
        {"cool",":/resources/images/emoji/SunglassesEmoji.png"},
        {"angry",":/resources/images/emoji/VeryAngryEmoji.png"},
};

Chat::Chat(QVBoxLayout* chatLayout):layout(chatLayout) {
    resignButton = new QPushButton("Resign");
    connect(resignButton,&QPushButton::clicked, this ,[this]() {
        emit this->playerResigned();
    });
    messageBox = new QListWidget();
    messageBox->setStyleSheet(
            "QListWidget::item {"
            "   height: 20px;"    // Adjust item height
            "   font-size: 60px;" // Adjust font size
            "   padding: 10px;"   // Adjust padding
            "   font-weight: bold;"   // Adjust padding
            "}"
            "QListWidget::item:selected {"
            "   background-color: #c0c0c0;" // Example selection color
            "   color: #ffffff;"             // Example selection text color
            "}"
    );
    layout->addWidget(resignButton);
    layout->addWidget(messageBox);

    auto buttonBoxRow1 = new QHBoxLayout();
    auto buttonBoxRow2 = new QHBoxLayout();
    auto emojiPathes = Chat::allEmoji.values();
    auto emojiNames = Chat::allEmoji.keys();
    for (int i = 0; i < 6; ++i) {
        auto button = new QPushButton(QIcon(emojiPathes[i]),"");
        button->setProperty("emojiName",emojiNames[i]);
        button->setCursor(Qt::PointingHandCursor);
        button->setMaximumWidth(50);
        if(i < 3){
            buttonBoxRow1->addWidget(button);

        }else{
            buttonBoxRow2->addWidget(button);
        }
        connect(button, &QPushButton::clicked, this, [this, button]() {
            auto buttonIcon = button->icon();
            auto newMessage = new QListWidgetItem(QIcon(buttonIcon)," : " + Cookie::getInstance()->loggedInPlayer->getUsername() );
            newMessage->setTextAlignment(Qt::AlignRight);
            this->messageBox->addItem(newMessage);
            emit this->sendMessageToOpponent(button->property("emojiName").toString());
        });
    }
    layout->addLayout(buttonBoxRow1);
    layout->addLayout(buttonBoxRow2);
}

Chat::~Chat() {
    delete resignButton;
    messageBox->clear();
    delete messageBox;
}

void Chat::addIconToChatBox(QString iconName,QString senderUsername) {
    if(!Chat::allEmoji.keys().contains(iconName)){
        return;
    }
    auto newMessage = new QListWidgetItem(QIcon(Chat::allEmoji[iconName])," : " + senderUsername);
    newMessage->setTextAlignment(Qt::AlignRight);
    this->messageBox->addItem(newMessage);
}
