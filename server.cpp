#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 6543);
    boost::asio::ip::tcp::acceptor acceptor(io_service, ep);
    // acceptor is now listening
    std::cout << "Listening on 6543" << std::endl;
    boost::asio::ip::tcp::socket socket(io_service);


    while (true) {
        boost::asio::streambuf req, filebuf, headerbuf;

        acceptor.accept(socket);
        // socket is now open
        std::cout << "Accepted connection\n";
        
        // Read HTTP request
        boost::asio::read_until(socket, req, "\r\n\r\n");

        std::string s(
                boost::asio::buffers_begin(req.data()),
                boost::asio::buffers_end(req.data()));

        std::cout << s << std::endl;

        std::ifstream fstr("./sample_html.html");
        std::ostream file_stream(&filebuf);
        file_stream << fstr.rdbuf();
        fstr.close();


        std::ostream header_stream(&headerbuf);
        header_stream << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
        header_stream << filebuf.size() << "\r\n\r\n";

        s = std::string(
                boost::asio::buffers_begin(headerbuf.data()),
                boost::asio::buffers_end(headerbuf.data()));

        std::cout << s << std::endl;

        s = std::string(
                boost::asio::buffers_begin(filebuf.data()),
                boost::asio::buffers_end(filebuf.data()));

        std::cout << s << std::endl;

        boost::asio::write(socket, headerbuf);
        boost::asio::write(socket, filebuf);
        socket.close();
    }
}
