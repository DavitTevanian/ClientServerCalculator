#include "Client.h"


int main()
{
    try
    {
        boost::asio::io_service aios;
        boost::asio::ip::tcp::resolver::query query("127.0.0.1", "8000");
        Client client(aios, query);
        client.start();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}