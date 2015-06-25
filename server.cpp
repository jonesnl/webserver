#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <netinet/in.h>

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

int main() {
    int accept_sock = socket(AF_INET, SOCK_STREAM, 0);
    int conn_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6438);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(accept_sock, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        cout << "Error binding\n";
        return -1;
    }
    
    listen(accept_sock, 5);

    while (true) {
        char rec_buf[10000], send_buf[10000], file_buf[10000];
        memset(rec_buf, 0, sizeof(rec_buf));
        memset(send_buf, 0, sizeof(send_buf));
        memset(file_buf, 0, sizeof(file_buf));
        conn_sock = accept(accept_sock, NULL, NULL);
        if (conn_sock == -1) return -1;
        cout << "Accepted connection\n";
        
        // Read HTTP request
        ssize_t end = 0;
        char *end_of_header = NULL;
        while((end_of_header = strstr(rec_buf, "\r\n\r\n")) == NULL) {
            end += recv(conn_sock, rec_buf + end, sizeof(rec_buf) - end - 1, 0);
        }
        if (end_of_header == NULL) {
            cout << "Error!\n";
            close(conn_sock);
            continue;
        }
        // size_t buf_len = end_of_header - rec_buf + 4;

        cout << rec_buf << endl;

        // Get content from file
        // TODO: Change hardcoded filename
        char *file_end = file_buf;
        int fd = open("./sample_html.html", O_RDONLY);
        if (fd == -1) return -1;
        ssize_t ret;
        do {
            ret = read(fd, file_end, sizeof(file_buf) - (file_end-file_buf));
            file_end += ret;
        } while (ret != 0 && ret != -1);
        
        long content_length = file_end - file_buf;
        string cont_len = to_string(content_length);

        // Store HTTP response header in buf
        char *headerptr = send_buf;
        // TODO set Content-Type on the fly
        char first_line[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
        memcpy(headerptr, first_line, sizeof(first_line) - 1);
        headerptr += sizeof(first_line) - 1;
        memcpy(headerptr, cont_len.c_str(), cont_len.size());
        headerptr += cont_len.size();
        memcpy(headerptr, "\r\n\r\n", 4);
        headerptr += 4;
        
        // Store content in buf
        memcpy(headerptr, file_buf, content_length);
        headerptr += content_length;
        cout << send_buf << endl;
        char* sendptr = send_buf;

        // Send buf back to browser
        while (sendptr != headerptr) {
            sendptr += send(conn_sock, sendptr, headerptr - sendptr, 0);
        }
        close(conn_sock);
    }
}
