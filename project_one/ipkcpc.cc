#include <iostream>
#include <string>
#include <regex>
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
    


    return EXIT_SUCCESS;
}