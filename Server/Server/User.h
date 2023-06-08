#pragma once
#include <memory>
#include "SHA1.h"
#include "ChatBox.h"

class ChatBox;

class User
{
public:
    User(const std::string& login, const std::string& password);
    User(const std::string& login, const std::vector<uint>& hash);
    ~User();
    std::string GetName() const;
    std::vector<uint> GetPassword() const;
    bool isPasswordCorrect(const std::string& password) const;
    void SetPassword(const std::string& password);
    int GetID() const;

    bool GetStatus() const;
    int GetSocket() const;
    std::shared_ptr<ChatBox> GetChat() const;
    void SetStatus(bool status);
    void SetSocket(const int socket);
    void SetChat(const std::shared_ptr<ChatBox> chat);
private:
    std::string m_login;

    std::vector<uint> m_hpassword;
    static int ID_count;
    int ID;
    bool m_status;
    std::shared_ptr<ChatBox> m_CHAT;
    int m_socket;
};
