#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

class Session
{
public:
    Session(boost::asio::io_service& io_service);
    boost::asio::ip::tcp::socket& socket() { return socket_; }
    void start();
    
private:
    void handle_read(const boost::system::error_code& error);
    void handle_write(const boost::system::error_code& error);
    void parser();
    struct Result
    {
        Result(double res, int state) : res_(res), state_(state) {}
        double res_;
        int    state_;
    };
    Result calculate(const double& arg1, const char op, const double& arg2);

private:
    boost::asio::ip::tcp::socket socket_;
    enum states_ { PASS, DIV_0, ERR_OP };
    enum { max_length = 1024 };
    char data_[max_length] = {};
};


class Server
{
public:
    Server(boost::asio::io_service& io_service, short port);

private:
    void start_accept();
    void handle_accept(Session* new_session, const boost::system::error_code& error);

private:
    boost::asio::io_service&       io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
};