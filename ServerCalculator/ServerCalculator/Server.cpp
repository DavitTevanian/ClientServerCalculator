#include "Server.h"

Session::Session(boost::asio::io_service& io_service) : socket_(io_service) {}

void Session::start()
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&Session::handle_read, this,
                            boost::asio::placeholders::error));
}

void Session::handle_read(const boost::system::error_code& error)
{
    if (!error)
    {
        parser();
        boost::asio::async_write(socket_, boost::asio::buffer(data_, max_length),
                                 boost::bind(&Session::handle_write, this,
                                 boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void Session::handle_write(const boost::system::error_code& error)
{
    (!error) ? start() : delete this;
}

void Session::parser()
{
    std::stringstream ss;
    ss.write(data_, max_length);
    double arg1;
    double arg2;
    char   op;
    ss >> arg1 >> op >> arg2;
    ss.str("");
    Result res = calculate(arg1, op, arg2);
    (res.state_ == PASS) ? ss << res.res_ : (res.state_ == DIV_0) ? ss << "Divide by zero !" : ss << "Incorrect operator !";
    size_t length = ss.str().copy(data_, ss.str().size());
    data_[length] = '\0';
}

Session::Result Session::calculate(const double& arg1, const char op, const double& arg2)
{
    switch (op)
    {
    case '+':
        std::cout << "ADD(" << arg1 << ", " << arg2 << ") = " << arg1 + arg2 << std::endl;
        return Result(arg1 + arg2, PASS);
    case '-':                                                               
        std::cout << "SUB(" << arg1 << ", " << arg2 << ") = " << arg1 - arg2 << std::endl;
        return Result(arg1 - arg2, PASS);
    case '*':                                                               
        std::cout << "MUL(" << arg1 << ", " << arg2 << ") = " << arg1 * arg2 << std::endl;
        return Result(arg1 * arg2, PASS);
    case '/':    
        if (arg2 == 0)
        {
            std::cout << "Divide by zero !\n";
            return Result(0.0, DIV_0);
        }
        else
        {
            std::cout << "DIV(" << arg1 << ", " << arg2 << ") = " << arg1 / arg2 << std::endl;
            return Result(arg1 / arg2, PASS);
        }
    default: 
        std::cout << "Incorrect operator !\n";
        return Result(0.0, ERR_OP);
    }
}

Server::Server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
    acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    start_accept();
}

void Server::start_accept()
{
    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(), boost::bind(&Server::handle_accept, this,
                           new_session, boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error)
{
    std::cout << "Server is ready\n";
    !error ? new_session->start() : delete new_session;
    start_accept();
}