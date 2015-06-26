#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 6543);
    boost::asio::ip::tcp::acceptor acceptor(io_service, ep);
    // acceptor is now listening
    std::cout << "Listening on 6543" << std::endl;
    boost::asio::ip::tcp::socket socket(io_service);


    while (true) {
        boost::asio::streambuf req;

        acceptor.accept(socket);
        // socket is now open
        std::cout << "Accepted connection\n";
        
        // Read HTTP request
        size_t req_size = boost::asio::read_until(socket, req, "\r\n\r\n");

        boost::asio::streambuf::const_buffers_type bufs = req.data();
        std::string s(
                boost::asio::buffers_begin(bufs),
                boost::asio::buffers_begin(bufs) + req_size);

        std::cout << s;
        std::cout << std::endl;

        socket.close();

        // // Get content from file
        // // TODO: Change hardcoded filename
        // char *file_end = file_buf;
        // int fd = open("./sample_html.html", O_RDONLY);
        // if (fd == -1) return -1;
        // ssize_t ret;
        // do {
        //     ret = read(fd, file_end, sizeof(file_buf) - (file_end-file_buf));
        //     file_end += ret;
        // } while (ret != 0 && ret != -1);
        // 
        // long content_length = file_end - file_buf;
        // string cont_len = to_string(content_length);

        // // Store HTTP response header in buf
        // char *headerptr = send_buf;
        // // TODO set Content-Type on the fly
        // char first_line[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
        // memcpy(headerptr, first_line, sizeof(first_line) - 1);
        // headerptr += sizeof(first_line) - 1;
        // memcpy(headerptr, cont_len.c_str(), cont_len.size());
        // headerptr += cont_len.size();
        // memcpy(headerptr, "\r\n\r\n", 4);
        // headerptr += 4;
        // 
        // // Store content in buf
        // memcpy(headerptr, file_buf, content_length);
        // headerptr += content_length;
        // cout << send_buf << endl;
        // char* sendptr = send_buf;

        // // Send buf back to browser
        // while (sendptr != headerptr) {
        //     sendptr += send(conn_sock, sendptr, headerptr - sendptr, 0);
        // }
        // close(conn_sock);
    }
}
