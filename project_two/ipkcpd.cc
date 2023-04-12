#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <regex>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

using namespace std;

#define TCP 0
#define UDP 1
#define UDP_LIMIT 256   // max # of data udp can send
#define TCP_LIMIT 65535 // max # of data tcp can send
#define OPCODE_REQUEST 0
#define OPCODE_RESPONSE 1
#define STATUS_OKEY 0
#define STATUS_ERROR 1
#define REQUEST_OFFSET 2  // offset for reading from udp request
#define RESPONSE_OFFSET 3 // offset for writing into udp response

int client_socket; // global variables for signal handling
int mode;
bool close_soc = false;

void print_usage(void)
{
    cout << "ipkcpd -h <host> -p <port> -m <mode>" << endl;
    cout << "where host is the IPv4 address the server will listen on, port" << endl;
    cout << "the port it will listen on, and mode either tcp or udp (e.g., ipkcpd -h 127.0.0.1 -p 2023 -m udp)" << endl;
    cout << "The server will use the appropriate protocol variant depending on the selected mode (binary for UDP, textual for TCP)" << endl;
    cout << "It should be able to handle more than one client at the same time." << endl;
}
/**
 * @brief printd msg to stderr , if socket was opened closes it
 *
 * @param msg to be printed out
 */
void exit_err(string msg)
{
    cerr << msg << endl;
    if (close_soc)
        close(client_socket);
    exit(EXIT_FAILURE);
}
/**
 * @brief handler for sigint,if mode is TCP, says BYE to server , if socket was opened closes it
 *
 * @param signum
 */
void signal_callback_handler(int signum)
{

    if (mode == TCP)
    {
        send(client_socket, "BYE\n", strlen("BYE\n"), 0); // if tcp, send BYE
        cout << "BYE\n";

        char buffer[TCP_LIMIT];
        recv(client_socket, buffer, TCP_LIMIT, 0); // and wait for BYE from server
        cout << buffer;
    }
    if (close_soc)
        close(client_socket);
    // terminate program
    exit(signum);
}