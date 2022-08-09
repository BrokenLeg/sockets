#pragma once

//TCP socket

class b_socket
{
public:
    b_socket(const char* port, const char* hostname, bool is_serv=false);
    virtual ~b_socket();
    int sd;
};

class b_socket_server : public b_socket
{   
public:

    b_socket_server(const char* port);
    virtual ~b_socket_server();

    void b_listen(int users_num);
    int b_accept();
    void run();
};

