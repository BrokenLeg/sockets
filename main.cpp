#include "b_socket.hpp"

#include <iostream>
#include <cstring>

int main()
{

    {
    b_socket_server sck("8080");
    sck.b_listen(100);
    sck.run();
    }

    return 0;
}