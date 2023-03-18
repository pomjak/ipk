#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <regex>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#define UDP_LIMIT 255
#define TCP_LIMIT 65535
#define OPCODE_REQUEST 0
#define OPCODE_RESPONSE 1
#define STATUS_OKEY 0
#define STATUS_ERROR 1
#define OPCODE_OFFSET 2
#define RESPONSE_OFFSET 3

using namespace std;

void print_usage(void)
{
    cout << "The client is started using: ipkcpc -h <host> -p <port> -m <mode>\n";
    cout << "where host is the IPv4 address of the server, port the server port,\n";
    cout << "and mode either tcp or udp (e.g., ipkcpc -h 1.2.3.4 -p 2023 -m udp)\n";
}

void exit_err(string msg)
{
    cerr << msg << endl;
    exit(EXIT_FAILURE);
}

void signal_callback_handler(int signum)
{
    cout << "Exiting gracefully " << endl;
    // Terminate program
    exit(signum);
}

int main(int argc, char *argv[])
{
    string host_ip,port_num,mode;

    signal(SIGINT, signal_callback_handler);

    if(argc == 2 && string(argv[1]) == "--help")
        print_usage();

    else if(argc == 7)
    {
        for(int i = 1; i < argc; i++)
        {
            if(string(argv[i]) == "-h")
            {
                host_ip = argv[++i];
                regex reg("^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])(.(?!$)|$)){4}$");//regex fot ip
                if(!regex_match(host_ip,reg))// if not match exit
                    exit_err("poorly formated ip addr");
            }

            else if (string(argv[i]) == "-p")
            {
                port_num = argv[++i];
                if (stoi(port_num) < 1 || stoi(port_num) > 65535)//ports num can be <1,65535>,
                    exit_err("poorly formated port number");
            }

            else if (string(argv[i]) == "-m")
            {
                mode = argv[++i];
                regex reg("^(tcp|udp)$");
                if (!regex_match(mode, reg))
                    exit_err("poorly formated mode");
            }
            else exit_err("poorly chosen args");
        }
    }

    else exit_err("bad # of args");
    
    if(mode == "udp")
    {
        /*
         * socket()
         */
        int client_socket;
        if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating client socket");

        /*
         * bind()
         */
        struct sockaddr_in server_addr; // struct fot server information

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(stoi(port_num));

        if (inet_pton(AF_INET, host_ip.c_str(), &server_addr.sin_addr) <= 0) // validating ip addr of server
            exit_err("Invalid ip addr");

        char buffer[UDP_LIMIT] = "\0";

        while (fgets(buffer, UDP_LIMIT, stdin))
        {
            buffer[strlen(buffer) - 1] = '\0';
            char temp[UDP_LIMIT] = {OPCODE_REQUEST, (char)strlen(buffer)};

            strcat(temp + OPCODE_OFFSET, buffer);

            memcpy(buffer, temp, UDP_LIMIT);

            /*
             * sendto()
             */
            int bytes_tx = sendto(client_socket, buffer, strlen(buffer + OPCODE_OFFSET) + OPCODE_OFFSET, MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (bytes_tx < 0)
                exit_err("send to FAILED");

            socklen_t len;
            char srv_response[UDP_LIMIT];
            int bytes_rx = recvfrom(client_socket, (char *)srv_response, UDP_LIMIT, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
            if (bytes_rx < 0)
                exit_err("recv to FAILED");

            srv_response[bytes_rx] = '\0';

            if (srv_response[0] != OPCODE_RESPONSE)
                exit_err("bad server response opcode");

            if (srv_response[1] != STATUS_OKEY)
                cout << "ERR:" << srv_response + RESPONSE_OFFSET << endl;
            else
                cout << "OK:" << srv_response + RESPONSE_OFFSET << endl;
        }

        close(client_socket);
    }
    else
    {
        /*
         * socket()
         */
        int client_socket;
        if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating client socket");
        

        struct sockaddr_in server_addr;

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(stoi(port_num));

        if (inet_pton(AF_INET, host_ip.c_str(), &server_addr.sin_addr) <= 0) // validating ip addr of server
            exit_err("Invalid ip addr");

        if (connect(client_socket, (const sockaddr *)&server_addr, sizeof(server_addr)) != 0)
            exit_err("connection with server failed");

        char buffer[TCP_LIMIT];

        while (fgets(buffer, TCP_LIMIT, stdin))
        {
            
            int bytes_tx = send(client_socket, buffer, strlen(buffer), 0);
            if(bytes_tx < 0)
                exit_err("send failed");

            memset(buffer, 0, sizeof(buffer));

            int bytes_rx = recv(client_socket, buffer, TCP_LIMIT, 0);
            if (bytes_rx < 0)
                exit_err("recv failed");
            else
            {
                cout << buffer;
                
                if (!strcmp(buffer,"BYE\n"))
                    break;
            }
            memset(buffer, 0, sizeof(buffer));
        }
        close(client_socket);
    }

    return EXIT_SUCCESS;
}