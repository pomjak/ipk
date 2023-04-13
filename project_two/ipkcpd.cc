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

int srv_socket; // global variables for signal handling
int mode;
bool close_soc = false;

int main(int argc, char *argv[])
{
    string ip;
    unsigned int port_num = 0;

    arg_processing(argc,argv,ip,&port_num);

    set_up_socket();

    bind(port_num);

    if(mode == UDP)
        udp_communication();

    else if(mode == TCP){}
        // TCP COMM

    else exit_err("main:unrecognized mode");


    return EXIT_SUCCESS;
}