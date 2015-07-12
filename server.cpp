#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <regex>

#include <boost/asio.hpp>

std::string parse_http_header(std::string header) {
    std::regex regex("^GET (/\\S*) HTTP/1\\.1\r\n");
    std::smatch results;
    bool ret = std::regex_search(header, results, regex);
    if (ret && results[1].str().find("..") == std::string::npos) {
        return results[1];
    } else {
        return std::string();
    }
}

std::string mime_type(std::string file_name) {
    std::regex regex("\\S+\\.(\\S+)$");
    std::smatch result;
    bool ret = std::regex_match(file_name, result, regex);
    if (ret) {
        std::string extension = result[1];
        if (extension == "html") return "text/html";
        else if (extension == "ogv") return "video/ogg";
        else return "text/html";
    } else return "text/html";
}

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

        // Parse request
        std::string file_name = parse_http_header(s);
        std::string m_type = mime_type(file_name);

        // Get data from file
        std::ifstream fstr("." + file_name);
        std::ostream file_stream(&filebuf);
        file_stream << fstr.rdbuf();
        fstr.close();

        // Create response header
        std::ostream header_stream(&headerbuf);
        header_stream << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: " << m_type << "\r\n"
            << "Content-Length: " << filebuf.size() << "\r\n"
            << "\r\n";

        s = std::string(
                boost::asio::buffers_begin(headerbuf.data()),
                boost::asio::buffers_end(headerbuf.data()));

        std::cout << s << std::endl;

        s = std::string(
                boost::asio::buffers_begin(filebuf.data()),
                boost::asio::buffers_end(filebuf.data()));

        std::cout << s << std::endl;

        // Send header and file back to requester
        boost::asio::write(socket, headerbuf);
        boost::asio::write(socket, filebuf);
        socket.close();
    }
}
