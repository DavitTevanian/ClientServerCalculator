#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

class Client
{
public:
    Client(boost::asio::io_service& aios, const boost::asio::ip::tcp::resolver::query& query);
    void start();

private:
    boost::asio::io_service&              aios_;
    boost::asio::ip::tcp::resolver::query query_;
    boost::asio::ip::tcp::socket          socket_;

    enum { max_length = 1024 };
    char request_[max_length] = {};
    char result_[max_length]  = {};
};