#include "Shell.h"

int User::ID_count = 0;

void send_advanced(int socket_descriptor, std::string& str, int flag)
{
    size_t message_size = str.length();
    send(socket_descriptor, (char*)&message_size, sizeof(int), flag);
    send(socket_descriptor, str.c_str(), message_size, flag);
}

Shell::Shell()
{
    m_users.init();
    for (int i = 0; i < m_users.GetUsersNumber(); ++i)
    {
        m_links.AddUser();
    }


    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(55555);
    server_addr.sin_family = AF_INET;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cout << "Error creating socket\n";
        exit(1);
    }

    bind_status = bind(server_socket, (sockaddr*)(&server_addr), sizeof(server_addr));
    if (bind_status == -1)
    {
        std::cout << "Error binding socket\n";
        exit(2);
    }

    connection_status = listen(server_socket, SOMAXCONN);
    if (connection_status == -1)
    {
        std::cout << "Error listening\n";
        exit(3);
    }
    else
    {
        std::cout << "Awaiting for connection\n";
    }
    FD_SET(server_socket, &master);
    fdmax = server_socket;

    std::cout << "Shell " << this << " created\n";
}

Shell::~Shell()
{
#ifdef WIN32
    closesocket(server_socket);
    WSACleanup();
#else
    close(server_socket);
#endif
    std::cout << "Shell " << this << " deleted\n";
}

void Shell::work()
{
    while (1)
    {
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cout << "Error calling select() function\n";
            exit(4);
        }

        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == server_socket)
                {

                    addrlen = sizeof(remoteaddr);
                    newfd = accept(server_socket, (struct sockaddr*)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        std::cout << "Connection accepting error\n";
                    }
                    else
                    {
                        std::cout << "Connection established. Socket descriptor: " << newfd << "\n";
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                    }
                }
                else
                {

                    if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0)
                    {
                        if (nbytes == 0)
                        {
                            std::cout << "Connection closed. Socket descriptor: " << i << "\n";
                        }
                        else
                        {
                            std::cout << "Receiving error from " << i << "\n";
                        }
                        std::shared_ptr<User> User_to_offline = m_users.getUserBySocket(i);
                        if (User_to_offline != nullptr)
                        {
                            m_users.setChatByName(User_to_offline->GetName(), nullptr);
                            m_users.setStatusByName(User_to_offline->GetName(), false);
                            m_users.setSocketByName(User_to_offline->GetName(), -1);
                        }

#ifdef WIN32
                        closesocket(i);
                        WSACleanup();
#else
                        close(i);
#endif
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        std::cout << "Msg from " << i << ":   " << buf << "\n";

                        std::string msg;
                        std::string msg_back;
                        for (int k = 0; k < bufsize; ++k)
                        {
                            if (buf[k] == '\0') break;
                            msg.push_back(buf[k]);
                        }

                        std::shared_ptr<User> currentUser = m_users.getUserBySocket(i);
                        if (currentUser == nullptr)
                        {

                            if ((msg[0] == '/') && (msg[1] == 'r') && (msg[2] == ' '))
                            {
                                size_t found = msg.find(' ', 3);
                                std::string name = msg.substr(3, found - 3);
                                std::string password = msg.substr(found);
                                if (m_users.IsLoginFree(name))
                                {
                                    m_users.AddUser(name, password);
                                    m_links.AddUser();
                                    msg_back = "User with name: " + name + " and password: " + password + " created!";

                                    send_advanced(i, msg_back);
                                }
                                else
                                {
                                    msg_back = "Login is already in use...";

                                    send_advanced(i, msg_back);
                                }
                            }
                            else if ((msg[0] == '/') && (msg[1] == 'l') && (msg[2] == ' '))
                            {
                                size_t found = msg.find(' ', 3);
                                std::string name = msg.substr(3, found - 3);
                                std::string password = msg.substr(found);
                                if (m_users.IsLoginFree(name))
                                {
                                    msg_back = "No user with the name " + name;

                                    send_advanced(i, msg_back);

                                }
                                else
                                {
                                    if (m_users.isPasswordCorrect(name, password))
                                    {
                                        if (m_users.getUser(name)->GetStatus())
                                        {
                                            msg_back = "User " + name + " is already online!";
                                            send_advanced(i, msg_back);
                                        }
                                        else
                                        {
                                            msg_back = "Successfull login as " + name;
                                            send_advanced(i, msg_back);
                                            currentUser = m_users.getUser(name);
                                            m_users.setChatByName(name, m_links.GetMain());
                                            m_users.setStatusByName(currentUser->GetName(), true);
                                            m_users.setSocketByName(currentUser->GetName(), i);
                                        }
                                    }
                                    else
                                    {
                                        msg_back = "Wrong password!";

                                        send_advanced(i, msg_back);
                                    }
                                }
                            }
                            else
                            {
                                msg_back = "Unknown command. Only /r and /l are awaliable";

                                send_advanced(i, msg_back);
                            }
                        }
                        else
                        {

                            if ((msg[0] == '/'))
                            {
                                if ((msg[1] == 's') && (msg[2] == ' '))
                                {
                                    std::string name = msg.substr(3);
                                    msg_back = "Now you are writing to " + name;

                                    send_advanced(i, msg_back);

                                    std::shared_ptr<ChatBox> ChatToWrite = m_links.Get(currentUser->GetID(), m_users.GetIDbyName(name));
                                    m_users.setChatByName(currentUser->GetName(), ChatToWrite);
                                }
                                else if ((msg[1] == 'a') && (msg[2] == 'l') && (msg[3] == 'l'))
                                {
                                    msg_back = "Now you are writing to main chat";

                                    send_advanced(i, msg_back);
                                    std::shared_ptr<ChatBox> ChatToWrite = m_links.GetMain();
                                    m_users.setChatByName(currentUser->GetName(), ChatToWrite);
                                }
                                else if ((msg[1] == 'g') && (msg[2] == 'e') && (msg[3] == 't'))
                                {
                                    msg_back = "CONTENTS OF CURRENT CHAT:\n";

                                    send_advanced(i, msg_back);

                                    std::vector<std::string>* localMessages = currentUser->GetChat()->getContainer();
                                    for (size_t k = 0; k < localMessages->size(); ++k)
                                    {
                                        send_advanced(i, localMessages->at(k));
                                        std::cout << localMessages->at(k) << "\n";
                                    }

                                }
                                else if ((msg[1] == 'l') && (msg[2] == 'o') && (msg[3] == 'u') && (msg[4] == 't'))
                                {
                                    msg_back = "Good bye!";

                                    send_advanced(i, msg_back);

                                    m_users.setChatByName(currentUser->GetName(), nullptr);
                                    m_users.setStatusByName(currentUser->GetName(), false);
                                    m_users.setSocketByName(currentUser->GetName(), -1);
                                }
                                else
                                {
                                    msg_back = "No such command! Only /lout, /s UserName, /get and /all are avaliable.\n";

                                    send_advanced(i, msg_back);
                                }
                            }
                            else
                            {
                                std::shared_ptr<ChatBox> currentChat = currentUser->GetChat();
                                msg = "[" + currentUser->GetName() + "]:" + " " + msg;
                                currentChat->sendMsg(msg);
                                msg_back = "Successfull sending!";
                                send_advanced(i, msg_back);
                                std::unordered_map<std::string, std::shared_ptr<User> >* umap = currentChat->getUmap();
                                for (auto it = umap->begin(); it != umap->end(); ++it)
                                {
                                    if (it->second != currentUser)
                                    {
                                        int newSocket = it->second->GetSocket();
                                        send_advanced(newSocket, msg);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}