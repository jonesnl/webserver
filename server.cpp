#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <netinet/in.h>

#include <iostream>

using namespace std;

int main() {
    int accept_sock = socket(AF_INET, SOCK_STREAM, 0);
    int conn_sock = socket(AF_INET, SOCK_STREAM, proto_num);

    struct sockaddr_in saddr = {AF_INET, htons(6438), INADDR_ANY};
    bind(accept_sock, saddr, sizeof(saddr));
    listen(accept_sock, 5);

    while (true) {
        conn_sock = accept(accept_sock, NULL, NULL);
        if (conn_sock == -1) return -1;
        cout << "Accepted connection\n";
        // Read HTTP request
        // Give HTTP response header
        // Give Vorbis video
    }
}
