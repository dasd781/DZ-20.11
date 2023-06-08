#include "User.h"

User::User(const std::string& login, const std::string& password) : m_login(login)
{
    ID = ID_count;
    ++ID_count;
    std::string password_tmp = password;
    uint salt = static_cast<uint>(ID);
    password_tmp += std::to_string(salt);
    m_hpassword = sha1_str(password_tmp);
    sha1_add_salt(m_hpassword, salt);
    m_status = false;
    m_CHAT = nullptr;
    m_socket = -1;
    std::cout << "USER constructor " << this << " called\n";
}

User::User(const std::string& login, const std::vector<uint>& hash) : m_login(login), m_hpassword(hash)
{
    ID = ID_count;
    ++ID_count;
    m_status = false;
    m_CHAT = nullptr;
    m_socket = -1;
    std::cout << "User " << this << " added from disk storage\n";
}

User::~User()
{
    std::cout << "USER destructor " << this << " called\n";
}
std::string User::GetName() const { return m_login; }

std::vector<uint> User::GetPassword() const { return m_hpassword; }

bool User::isPasswordCorrect(const std::string& password) const
{
    std::string password_tmp = password;
    uint salt = sha1_pick_salt(m_hpassword);
    password_tmp += std::to_string(salt);
    std::vector<uint> hash_to_check = sha1_str(password_tmp);
    bool compare = two_hashes_compare(m_hpassword, hash_to_check, hash_size);
    return compare;
};

void User::SetPassword(const std::string& password)
{
    m_hpassword = sha1_str(password);
};

int User::GetID() const { return ID; };

bool User::GetStatus() const { return m_status; };

int User::GetSocket() const { return m_socket; };

std::shared_ptr<ChatBox> User::GetChat() const { return m_CHAT; };

void User::SetStatus(bool status) { m_status = status; };

void User::SetSocket(const int socket) { m_socket = socket; };

void User::SetChat(const std::shared_ptr<ChatBox> chat)
{
    m_CHAT = chat;
};