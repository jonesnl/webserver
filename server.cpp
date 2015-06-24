#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <netinet/in.h>

#include <iostream>
#include <cstring>

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
        char rec_buf[10000];
        memset(rec_buf, 0, sizeof(rec_buf));
        conn_sock = accept(accept_sock, NULL, NULL);
        if (conn_sock == -1) return -1;
        cout << "Accepted connection\n";
        
        // Read HTTP request
        int end = 0;
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
        close(conn_sock);
        // Give HTTP response header
        // Give Theora video
    }
}
