#pragma once
#include "ChatBox.h"
#include <memory>

class LinkStorage
{
public:
    LinkStorage();
    ~LinkStorage();
    void AddUser();
    std::shared_ptr<ChatBox> Get(int ID1, int ID2) const;
    std::shared_ptr<ChatBox> GetMain() const;
    void PrintAddresses() const;
private:

    std::vector<std::vector<std::shared_ptr<ChatBox> > > m_storage;
    std::shared_ptr<ChatBox> m_mainPtr;
    int m_count;
};
