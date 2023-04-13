#include "ipkcpd.h"
#include "udp_parser.h"
#include "udp_lexer.h"

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

void set_up_socket(void)
{
    if (mode == UDP)
    {
        if ((srv_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating server socket");
    }
    else
    { 
        if ((srv_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) // creating client socket for udp
            exit_err("failed creating server socket");
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
        cout << "request ok" << endl;

        payload_len = (int)request[1];

        request[REQUEST_OFFSET + payload_len + 1] = '\0';

        cout << "REQ:" << request + REQUEST_OFFSET << endl;
        
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

string calculate(string input)
{
    string int_2_str;

    
    Lexer lexer(input);

    Parser parser(lexer);

    int result = parser.parseQuery();

    int_2_str = to_string(result);   // convert from int to string 

    return int_2_str;
}

void udp_communication()
{
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    struct sockaddr *addr = (struct sockaddr *)&client_addr;

    char buffer[UDP_LIMIT] = "\0";

    while (true)
    {
        if (recvfrom(srv_socket, buffer, UDP_LIMIT, 0, addr, &addr_size) < 0)
            exit_err("ERROR: recvfrom");

        verify_request(buffer);

        string clean_exp(buffer + REQUEST_OFFSET);
        string result;
        char response[UDP_LIMIT];
        bool err = false;

        try
        {
            result = calculate(clean_exp); // calculate and return result
        }

        catch (const runtime_error &e)
        {
            cerr << e.what() << endl;
            format_response(response, STATUS_ERROR, "Could not parse that");
            err = true;
        }

        if(!err)
            format_response(response, STATUS_OKEY, result);

        if (sendto(srv_socket, response, strlen(response + RESPONSE_OFFSET) + RESPONSE_OFFSET, 0, addr, addr_size) < 0)
            exit_err("ERROR: sendto");
    }
}