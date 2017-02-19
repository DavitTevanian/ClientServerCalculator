#include "Client.h"
#include <boost/bind.hpp>

Client::Client(boost::asio::io_service& aios, const boost::asio::ip::tcp::resolver::query& query)
    : aios_(aios),
    socket_(aios),
    query_(query)
{}

void Client::start()
{
    boost::asio::ip::tcp::resolver resolver(aios_);
    boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query_);
    boost::asio::connect(socket_, endpoint);

    while (true)
    {
        std::cout << "[Calculator]$ ";      
        std::cin.getline(request_, max_length);
        boost::asio::write(socket_, boost::asio::buffer(request_, max_length));        
        size_t reply_length = boost::asio::read(socket_, boost::asio::buffer(result_, max_length));
        std::cout << result_ << std::endl;
    }
}
