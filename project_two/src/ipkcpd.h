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
#ifndef IPKCPD_H
#define IPKCPD_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <regex>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>

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
#define MAX_CLIENTS 32

extern int srv_socket; // global variables for signal handling
extern int mode;
extern bool close_soc ;
extern struct pollfd fds[MAX_CLIENTS];
extern int nfds;

void print_usage(void);

void exit_err(string msg);

void signal_callback_handler(int signum);

void arg_processing(int argc, char *argv[], string &ip, unsigned int *port);

void set_up_socket(void);

void bind(unsigned int port);

int verify_request(char *request);

void format_response(char *response, bool status, string msg);

string calculate(string input);

void udp_communication(void);

string tcp_verify(char *request, bool *close);

string tcp_calculate(char *request,bool *close);

void tcp_communication(void);

#endif