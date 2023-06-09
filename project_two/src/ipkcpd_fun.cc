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
#include "ipkcpd.h"
#include "parser.h"
#include "lexer.h"
#include "frac.h"

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
        if (close_soc)
        {
            for (int i = 0; i < nfds; i++)
            {
                if (fds[i].fd >= 0)
                {
                    send(fds[i].fd, "BYE\n", strlen("BYE\n"), 0);
                    close(fds[i].fd);
                }
            }
        }
    }
    else
    {
        if (close_soc)
        {
            close(srv_socket);
        }
    }
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

void set_up_socket(void)
{
    if (mode == UDP)
    {
        if ((srv_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating server socket");
    }
    else
    { 
        int enable = 1;
        if ((srv_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating server socket");

        setsockopt(srv_socket, SOL_SOCKET, SO_REUSEADDR,(char *)&enable, sizeof(enable));
        // ioctl(srv_socket, FIONBIO, (char *)&enable);
        int flags = fcntl(srv_socket, F_GETFL, 0);
        fcntl(srv_socket, F_SETFL, flags | O_NONBLOCK);
    }   
        

    close_soc = true;
}

void bind(unsigned int port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(srv_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        exit_err("bind failed");
}

int verify_request(char *request)
{
    int payload_len = 0;

    if (request[0] == OPCODE_REQUEST)
    {
        payload_len = (int)request[1];

        request[REQUEST_OFFSET + payload_len + 1] = '\0';
    }

    else exit_err("recieved response");

    return payload_len; 
}

void format_response(char *response, bool status, string msg)
{
    char temp[UDP_LIMIT] = {OPCODE_RESPONSE, status, (char)msg.length()};

    strcat(temp + RESPONSE_OFFSET, msg.c_str()); // adding msg behind opcode ,status and lenght

    memcpy(response, temp, UDP_LIMIT); // response <== temp
}

void reverse_stack_order(stack<Frac> *stack)
{
    std::stack<Frac> reversedStack;
    while (!stack->empty())
    {
        reversedStack.push(stack->top());
        stack->pop();
    }
    *stack = reversedStack;
}

string calculate(string input)
{
    string int_2_str;

    Lexer lexer(input);

    Parser parser(lexer);

    Frac result = parser.parseExpr();

    if (result.get_num() < 0)
        throw runtime_error("CALCULATE: result is less than 0");

    int_2_str = to_string(result.get_num());//get int num from fraction and convert it into string
    
    if(result.is_frac())
    {
        return to_string( (result.get_num()/result.get_denom()) );
    }

    return int_2_str;
}

void udp_communication()
{
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    struct sockaddr *addr = (struct sockaddr *)&client_addr;

    char buffer[UDP_LIMIT] = {0,};

    while (true)
    {
        memset(buffer, '\0', UDP_LIMIT * sizeof(buffer[0]));
        if (recvfrom(srv_socket, buffer, UDP_LIMIT, 0, addr, &addr_size) < 0)
            exit_err("ERROR: recvfrom");

        verify_request(buffer);

        string clean_exp(buffer + REQUEST_OFFSET);
        string result;
        char response[UDP_LIMIT];
        memset(response, '\0', UDP_LIMIT * sizeof(buffer[0]));
        bool err = false;

        try
        {
            result = calculate(clean_exp); // calculate and return result
        }

        catch (const runtime_error &e)
        {
            format_response(response, STATUS_ERROR, e.what());
            err = true;
        }

        if(!err)
            format_response(response, STATUS_OKEY, result);

        if (sendto(srv_socket, response, strlen(response + RESPONSE_OFFSET) + RESPONSE_OFFSET, 0, addr, addr_size) < 0)
            exit_err("ERROR: sendto");
    }
}

string tcp_verify(char *request, bool *hello, bool *close)
{
    if(string(request) == "HELLO\n")
    {
        *hello = true;
        return "HELLO\n";
    }
    else 
    {
        *close = true;
        return "BYE\n";
    }
}

string tcp_calculate(char *request, bool *close)
{
    if (!strncmp(request, "SOLVE ", strlen("SOLVE ")))
    {
        string result = "RESULT ";
        string input = request + strlen("SOLVE ");
        try
        {
            result.append(calculate(input));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << endl;
            *close = true;
            return "BYE\n";
        }
        result.append("\n");
        return result;
    }
    else
    {
        *close = true;
        return "BYE\n";
    }
}

bool accept_new_socket(bool *end)
{
    int new_socket;

    new_socket = accept(srv_socket, NULL, NULL);
    if (new_socket < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            *end = true;
            cerr<<("accept() failed")<<endl;
            return false;
        }
    }

    fds[nfds].fd = new_socket;
    fds[nfds].events = POLLIN;
    nfds++;
    return true;
}

void compress_fds(bool *compress_flag, bool *hello, bool *close)
{
    *compress_flag = false;
    for (int i = 0; i < nfds; i++)
    {
        if (fds[i].fd == -1)
        {
            for (int j = i; j < nfds - 1; j++)
            {
                fds[j].fd = fds[j + 1].fd;
                close[j] = close[j + 1];
                hello[j] = hello[j + 1];
            }
            nfds--;
            i--;
        }
    }
}

void tcp_communication(void)
{
    int rc, curr = 0;
    bool end = false, compress = false;
    bool received_hello[MAX_CLIENTS] = {false};
    bool close_conn[MAX_CLIENTS] = {false};
    char buffer[TCP_LIMIT] = {0,};

    listen(srv_socket, MAX_CLIENTS);
    memset(fds, 0, sizeof(fds));

    fds[0].fd = srv_socket;
    fds[0].events = POLLIN;

    do
    {
        if (poll(fds, nfds, -1) < 0)
        {
            exit_err("poll() failed");
            end = true;
            break;
        }

        curr = nfds;
        for (int i = 0; i < curr; i++)
        {
            if (!fds[i].revents)
                continue;

            if (fds[i].fd == srv_socket)
            {
                if(!accept_new_socket(&end))
                    break;
            }

            else
            {
                close_conn[i] = false;

                memset(buffer, '\0', sizeof(buffer));
                rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (rc < 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        cerr << "recv() failed " << endl;
                        close_conn[i] = true;
                    }
                }

                if (!rc)
                    close_conn[i] = true;

                string response;

                if (!received_hello[i])
                    response = tcp_verify(buffer, &received_hello[i], &close_conn[i]);
                else
                    response = tcp_calculate(buffer, &close_conn[i]);

                if (send(fds[i].fd, response.c_str(), response.length(), 0) < 0)
                {
                    cerr << "send() failed" << endl;
                    close_conn[i] = true;
                }

                if (close_conn[i])
                {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    close_conn[i] = false;
                    received_hello[i] = false;
                    compress = true;
                }
            }
        }

        if (compress)
            compress_fds(&compress,received_hello,close_conn);

    }while (!end);

    for (int i = 0; i < nfds; i++)
    {
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }
}
