#ifndef IPKCPD_H
#define IPKCPD_H

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

extern int srv_socket; // global variables for signal handling
extern int mode;
extern bool close_soc ;

void print_usage(void);

void exit_err(string msg);

void signal_callback_handler(int signum);

void arg_processing(int argc, char *argv[], string &ip, unsigned int *port);

void set_up_socket(void);

void bind(unsigned int port);

int verify_request(char *request);

void format_response(char *response, bool status, string msg);

string calculate(string input);

void udp_communication();

#endif