#include "Server.h"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        Server s(io_service, 8000);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}