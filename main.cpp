#include "b_socket.hpp"

#include <iostream>
#include <cstring>

void f(char* req)
{
    std::cout << 1;
}

int main()
{

    b_socket_server sck("8080");
    sck.b_listen(3);
    sck.run(f);

    return 0;
}