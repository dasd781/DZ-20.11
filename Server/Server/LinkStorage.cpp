#include "LinkStorage.h"

LinkStorage::LinkStorage()
{

    m_mainPtr = std::make_shared<ChatBox>("ChatMain.txt");
    m_count = 0;

    m_storage.resize(1);
    m_storage[0].resize(1);

    m_storage[0][0] = nullptr;

    std::cout << "Link storage " << this << " created\n";
}

LinkStorage::~LinkStorage()
{
    std::cout << "Link storage " << this << " deleted\n";
}

void LinkStorage::AddUser()
{
    ++m_count;

    if (m_count == 1)
    {

        m_storage[0][0] = std::make_shared<ChatBox>("Chat00.txt");
        return;
    }
    else
    {

        m_storage.resize(m_count);
        for (size_t i = 0; i < m_storage.size(); ++i) { m_storage[i].resize(m_count); }

        std::string chat_name = "Chat" + std::to_string(m_storage.size() - 1) + std::to_string(m_storage.size() - 1) + ".txt";
        m_storage[m_storage.size() - 1][m_storage.size() - 1] = std::make_shared<ChatBox>(chat_name);
        for (size_t i = 0; i < m_storage.size() - 1; ++i)
        {

            chat_name = "Chat" + std::to_string(i) + std::to_string(m_storage.size() - 1) + ".txt";
            m_storage[i][m_storage.size() - 1] = std::make_shared<ChatBox>(chat_name);
            m_storage[m_storage.size() - 1][i] = m_storage[i][m_storage.size() - 1];
        }
    }
}

void LinkStorage::PrintAddresses() const
{
    if (m_count == 0) { std::cout << "adres\n"; return; }

    for (size_t i = 0; i < m_storage.size(); ++i)
    {
        for (size_t j = 0; j < m_storage.size(); ++j)
        {
            std::cout << m_storage[i][j] << "\t";
        }
        std::cout << "\n";
    }
}