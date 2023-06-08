#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <unordered_map>

#include "User.h"

class User;

class ChatBox
{
public:
    ChatBox();
    ChatBox(const std::string& name);
    ~ChatBox();
    void sendMsg(std::string& msg);
    void sendMsgFromFile(std::string& msg);
    void showChatAll() const;
    void showChatLast() const;
    void setChatStatus(bool val);
    bool getChatStatus() const;
    void addUserToChatList(std::shared_ptr<User> User_ch);
    void deleteUserFromChatList(std::shared_ptr<User> User_ch);
    std::vector<std::string>* getContainer(); 
        std::unordered_map<std::string, std::shared_ptr<User> >* getUmap();
private:
    std::vector<std::string> m_messages;
    bool active;
    std::string filename;
    std::fstream m_file;
    bool m_toggle;
    std::unordered_map<std::string, std::shared_ptr<User> > m_users_active;
};
