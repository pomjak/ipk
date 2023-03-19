/*
 * IPK Project 1: IPK Calculator Protocol Client
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

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #define SIGINT 2
#else
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

#include <sys/socket.h>
#include <signal.h>

#define TCP 0
#define UDP 1
#define UDP_LIMIT 256 //max # of data udp can send
#define TCP_LIMIT 65535 // max # of data tcp can send
#define OPCODE_REQUEST 0 
#define OPCODE_RESPONSE 1
#define STATUS_OKEY 0
#define STATUS_ERROR 1
#define REQUEST_OFFSET 2    //offset for writing into udp request 
#define RESPONSE_OFFSET 3   //offset for reading from udp response

using namespace std;
int client_socket; //global variables for signal handling
int mode;
bool close_soc = false;
/**
 * @brief prints usage
 * 
 */
void print_usage(void)
{
    cout << "ipkcpc -h [host ip] -p [port number] -m [mode]" << endl;
    cout << "The client is started using: ipkcpc -h <host> -p <port> -m <mode>"<< endl;
    cout << "where host is the IPv4 address of the server, port the server port"<< endl;
    cout << "and mode either tcp or udp (e.g., ipkcpc -h 1.2.3.4 -p 2023 -m udp)"<< endl;
}
/**
 * @brief printd msg to stderr , if socket was opened closes it
 * 
 * @param msg to be printed out
 */
void exit_err(string msg)
{
    cerr << msg << endl;
    if(close_soc)
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

    if(mode == TCP)
    {
        send(client_socket, "BYE\n", strlen("BYE\n"), 0);//if tcp, send BYE 
        cout << "BYE\n";

        char buffer[TCP_LIMIT];
        recv(client_socket, buffer, TCP_LIMIT, 0);//and wait for BYE from server
        cout << buffer;

    }
    if (close_soc)
        close(client_socket);
    // terminate program
    exit(signum);
}

int main(int argc, char *argv[])
{
    string host_ip;
    int port_num = 0;
    // close(client_socket);
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData); // initialize Winsock on windows
    #endif

    signal(SIGINT, signal_callback_handler);

    if(argc == 2 && string(argv[1]) == "--help")
        print_usage();

    else if(argc != 7)
        exit_err("bad # of args, use as ipkcpc -h [host ip] -p [port number] -m [mode]");

    for(int i = 1; i < argc; i++)//options processing
    {
        if(string(argv[i]) == "-h")
        {
            host_ip = argv[++i];
            regex reg("^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])(.(?!$)|$)){4}$");//regex fot ip
            if(!regex_match(host_ip,reg))// if not match exit
                exit_err("poorly formated ip addr 0.0.0.0 - 255.255.255.255");
        }

        else if (string(argv[i]) == "-p")
        {
            port_num = atoi(argv[++i]);
            if (port_num < 1 || port_num > 65535)//ports num can be <1,65535>,
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
    
    //init socket depending on chosen protocol
    if(mode == UDP)
    {
        if((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating client socket");
    }
    else
    {
        if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating client socket");
    } 
    close_soc = true;
        
    //bind()
    struct sockaddr_in server_addr; // struct fot server information

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);

    if (inet_pton(AF_INET, host_ip.c_str(), &server_addr.sin_addr) <= 0) // validating ip addr of server
        exit_err("Invalid ip addr");

    if (mode == UDP)
    {
        char buffer[UDP_LIMIT] = "\0";

        while (fgets(buffer, UDP_LIMIT, stdin))
        {
            buffer[strlen(buffer) - 1] = '\0';// removing \n
            char temp[UDP_LIMIT] = {OPCODE_REQUEST, (char)strlen(buffer)};

            strcat(temp + REQUEST_OFFSET, buffer);//adding input from stdin behind opcode and lenght 

            memcpy(buffer, temp, UDP_LIMIT);//buffer <== temp

            int bytes_tx = sendto(client_socket, buffer, strlen(buffer + REQUEST_OFFSET) + REQUEST_OFFSET, MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (bytes_tx < 0)
                exit_err("send to FAILED");

            socklen_t len;
            char srv_response[UDP_LIMIT];
            int bytes_rx = recvfrom(client_socket, (char *)srv_response, UDP_LIMIT, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
            if (bytes_rx < 0)
                exit_err("recv FAILED");

            srv_response[bytes_rx] = '\0';

            if (srv_response[0] != OPCODE_RESPONSE)
                exit_err("bad server response opcode");

            int payload_len = (int)srv_response[2];
            srv_response[RESPONSE_OFFSET + payload_len+1] = '\0';

            if (srv_response[1] != STATUS_OKEY)
                cout << "ERR:" << srv_response + RESPONSE_OFFSET << endl;//OFFSET skips 1st 3BYTES of data
            else
                cout << "OK:" << srv_response + RESPONSE_OFFSET << endl;
        }
    }

    else
    {
        if (connect(client_socket, (const sockaddr *)&server_addr, sizeof(server_addr)) != 0)
            exit_err("connection with server failed");

        char buffer[TCP_LIMIT];

        while (fgets(buffer, TCP_LIMIT, stdin))//load from stdin
        {
            
            int bytes_tx = send(client_socket, buffer, strlen(buffer), 0);//send buffer to srv
            if(bytes_tx < 0)
                exit_err("send failed");

            memset(buffer, 0, sizeof(buffer));//clean buffer to be able to recieve

            int bytes_rx = recv(client_socket, buffer, TCP_LIMIT, 0);//recv from srv
            if (bytes_rx < 0)
                exit_err("recv failed");
            else//if success
            {
                cout << buffer;
                
                if (!strcmp(buffer,"BYE\n"))//if recieved msg is bye exit
                    break;
            }
            memset(buffer, 0, sizeof(buffer));//clean buffer to be able to read from stdin
        }
        
    }

    close(client_socket);
    
    return EXIT_SUCCESS;
}