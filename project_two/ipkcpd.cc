/*
 * IPK Project 2: IOTA: Server for Remote Calculator
 * Author: xpomsa00
 * Copyright (C) 2023 Pomsar Jakub FIT VUT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

int srv_socket; // global variables for signal handling
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

void exit_err(string msg)
{
    cerr << msg << endl;
    if (close_soc)
        close(srv_socket);
    exit(EXIT_FAILURE);
}

void signal_callback_handler(int signum)
{
    if (mode == TCP)
    {
        send(srv_socket, "BYE\n", strlen("BYE\n"), 0); // if tcp, send BYE
        // SEND BYE

        char buffer[TCP_LIMIT];
        recv(srv_socket, buffer, TCP_LIMIT, 0); // and wait for BYE from server
        cout << buffer;
    }
    if (close_soc)
        close(srv_socket);
    // terminate program
    exit(signum);
}

void arg_processing(int argc, char *argv[], string &ip, unsigned int* port)
{
    if (argc == 2 && string(argv[1]) == "--help")
        print_usage();

    else if (argc != 7)
        exit_err("bad # of args, use as ipkcpd -h [host ip] -p [port number] -m [mode]");

    for (int i = 1; i < argc; i++) // options processing
    {
        if (string(argv[i]) == "-h")
        {
            ip = argv[++i];
            regex reg("^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])(.(?!$)|$)){4}$"); // regex fot ip
            if (!regex_match(ip, reg))                                      // if not match exit
                exit_err("poorly formated ip addr 0.0.0.0 - 255.255.255.255");
        }

        else if (string(argv[i]) == "-p")
        {
            *port = atoi(argv[++i]);
            if (*port < 1 || *port > 65535) // ports num can be <1,65535>,
                exit_err("poorly formated port number (1;65535)");
        }

        else if (string(argv[i]) == "-m")
        {
            i++;
            regex reg_udp("^udp$");
            regex reg_tcp("^tcp$");

            if (regex_match(argv[i], reg_udp))
                mode = UDP;
            else if (regex_match(argv[i], reg_tcp))
                mode = TCP;
            else
                exit_err("poorly formated mode (tcp|udp))");
        }
        else
            exit_err("poorly chosen args, use as ipkcpc -h [host ip] -p [port number] -m [mode]");
    }
}

int main(int argc, char *argv[])
{
    string ip;
    unsigned int port_num = 0;

    arg_processing(argc,argv,ip,&port_num);

    if ((srv_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // creating client socket for udp
        exit_err("failed creating server socket");

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_num);

    if (bind(srv_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_err("bind failed");

    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    struct sockaddr *addr = (struct sockaddr *)&client_addr;

    char buffer[UDP_LIMIT] = "\0";
    int bytes_rx, bytes_tx;

    
    bytes_rx = recvfrom(srv_socket, buffer, UDP_LIMIT, 0, addr, &addr_size);
    if (bytes_rx < 0)
        exit_err("ERROR: recvfrom");

    


    // bytes_tx = sendto(srv_socket, buffer, strlen(buffer), 0, addr, addr_size);
    // if (bytes_tx < 0)
    //     exit_err("ERROR: sendto");
    

    return EXIT_SUCCESS;
}