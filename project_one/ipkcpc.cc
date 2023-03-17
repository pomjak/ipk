#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <netinet/in.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define OPCODE_REQUEST 0

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

int main(int argc, char *argv[])
{
    string host_ip,port_num,mode;

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
    

    int client_socket;

    char buffer[BUFFER_SIZE] = "(+ 6 4)";
    char temp[BUFFER_SIZE] = {OPCODE_REQUEST, (char)strlen(buffer)};
    strcat(temp + 2, buffer);

    memcpy(buffer, temp, BUFFER_SIZE);

    cout<< buffer << endl;

    /*
     * socket()
     */
    if((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)//creating client socket for udp
        exit_err("failed creating client socket");

    /*
     * bind()
     */
    struct sockaddr_in server_addr;//struct fot server information

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(port_num));

    if(inet_pton(AF_INET,host_ip.c_str(),&server_addr.sin_addr) <= 0)//validating ip addr of server
        exit_err("Invalid ip addr");

    /*
     * sendto()
     */
    int bytes_tx = sendto(client_socket, buffer, strlen(buffer+2)+2, MSG_CONFIRM, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(bytes_tx < 0 )
        exit_err("send to FAILED");

    socklen_t len;
    char * buf_out = NULL;
    int bytes_rx = recvfrom(client_socket, buf_out, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);
    if (bytes_rx < 0)
        exit_err("recv to FAILED");

    buf_out[bytes_rx+1] = '\0';
    cout << buf_out<< endl;

    close(client_socket);

    return EXIT_SUCCESS;
}