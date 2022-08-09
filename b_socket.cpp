#include "b_socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <iostream>


b_socket::b_socket(const char* port, const char* hostname, bool is_serv)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    
    if (is_serv)
        hints.ai_flags = AI_PASSIVE;

    addrinfo *address;
    if (getaddrinfo(hostname, port, &hints, &address))
    {
        exit(1);
    }

    sd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (sd == -1)
    {
        exit(1);
    }

    if (is_serv)
        bind(sd, address->ai_addr, address->ai_addrlen);

}
b_socket::~b_socket()
{
    close(sd);
}

b_socket_server::b_socket_server(const char* port) : b_socket(port, 0, true)
{
   
}

b_socket_server::~b_socket_server()
{
    //
}

void b_socket_server::b_listen(int users_num)
{
    listen(b_socket::sd, users_num);
}

int b_socket_server::b_accept()
{
    sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);

    int socket_client = accept(b_socket::sd, (sockaddr*) &client_address, &client_len);

    if (socket_client == -1)
    {
        exit(1);
    }

    return socket_client;
}

void b_socket_server::run(std::function<void(char*)> dispatcher)
{
    fd_set master;
    FD_ZERO(&master);
    FD_SET(b_socket::sd, &master);
    
    int max_socket = b_socket::sd;

    while (1)
    {
        fd_set reads = master;
        select(max_socket+1, &reads, 0, 0, 0);

        for (int socket = 1; socket <= max_socket; socket++)
        {

            if (FD_ISSET(socket, &reads))
            {

                if (socket == b_socket::sd)
                {
                    //new user knocks-knocks, accept they

                    sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);

                    int socket_client = accept(b_socket::sd, (sockaddr*) &client_address, &client_len);

                    if (socket_client == -1)
                    {
                        exit(1);
                    }

                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                    {
                        max_socket = socket_client;
                    }
                }
                else
                {
                    //request from user-socket, response them

                    char req[1024];
                    int br = recv(socket, req, sizeof(req), 0);
                   
                    if (br == 0)
                    {
                        close(socket);
                        FD_CLR(socket, &master);
                    }

                    dispatcher(req);
                }
            }

        }
    }
}