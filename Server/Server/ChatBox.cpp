#include "ChatBox.h"

ChatBox::ChatBox()
{
    active = false;
    m_toggle = false;
    std::cout << "Chat box " << this << " created\n";
}

ChatBox::ChatBox(const std::string& name)
{
    filename = name;
    active = false;
    m_toggle = false;
    m_file.open(filename, std::ios::in | std::ios::out);
    m_file.close();
    std::cout << "Chat box " << this << " with name: " << filename << " created\n";
}

ChatBox::~ChatBox()
{
    std::cout << "Chat box " << this << " deleted\n";
}

void ChatBox::sendMsg(std::string& msg)
{
    m_messages.push_back(msg);

    m_file << msg << "\n";
}

void ChatBox::sendMsgFromFile(std::string& msg)
{
    m_messages.push_back(msg);
}

void ChatBox::showChatAll() const
{
    if (m_messages.size() == 0) { std::cout << "No messages here...\n"; return; }
    for (size_t i = 0; i < m_messages.size(); ++i)
    {
        std::cout << m_messages[i] << "\n";
    }
}

void ChatBox::showChatLast() const
{
    if (m_messages.size() == 0) return;
    std::cout << m_messages[m_messages.size() - 1] << "\n";
}

void ChatBox::setChatStatus(bool val)
{
    active = val;

    if (val == false)
    {
        std::cout << "Chat " << filename << " closed!\n";
        m_file.close();
    }
    else if (m_toggle == false)
    {

        m_file.open(filename, std::ios::in | std::ios::out);
        if (m_file.is_open())
        {
            std::cout << "Chat " << filename << " opened(toggle == false)!\n";
            m_file.seekg(0, std::ios_base::beg);
            while (!m_file.eof())
            {
                std::string msg;


                std::getline(m_file, msg);
                sendMsgFromFile(msg);
            }
            m_file.close();
        }
        m_toggle = true;
        m_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
    }
    else
    {
        std::cout << "Chat " << filename << " opened(toggle == true)!\n";
        m_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
    }
}

bool ChatBox::getChatStatus() const
{
    return active;
}

void ChatBox::addUserToChatList(std::shared_ptr<User> User_ch)
{
    m_users_active.insert(std::make_pair(User_ch->GetName(), User_ch));
    setChatStatus(true);
}

void ChatBox::deleteUserFromChatList(std::shared_ptr<User> User_ch)
{
    if (m_users_active.empty()) return;

    std::unordered_map<std::string, std::shared_ptr<User> >::iterator it = m_users_active.find(User_ch->GetName());
    if (it == m_users_active.end()) return;
    else
    {
        m_users_active.erase(User_ch->GetName());
        if (m_users_active.empty()) setChatStatus(false);
    }
}

std::vector<std::string>* ChatBox::getContainer()
{
    return &m_messages;
}

std::unordered_map<std::string, std::shared_ptr<User> >* ChatBox::getUmap()
{
    return &m_users_active;
}