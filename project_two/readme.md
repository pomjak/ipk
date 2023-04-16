## IPK Calculator Protocol Client

### Autor: Pomsár Jakub

### xlogin: xpomsa00
### Content
- Required theory
- Description of interesting parts 
- Testing section
- Description of features beyond the specification

### Required theory
In order to implement the program correctly, it is important to understand the differences between the TCP and UDP communication protocols.

TCP is a reliable protocol that guarantees the delivery of data without errors and in the correct order. This is achieved through the use of acknowledgments and retransmissions to ensure that all data is received correctly.

On the other hand, UDP is an unreliable protocol that does not guarantee the delivery or order of data. However, it is faster and more efficient than TCP.

Another important aspect to consider is the IPK calculator protocol, which offers both binary (UDP) and textual (TCP) variants. This protocol is intended for communication between servers and clients.

### Description of interesting parts 
For the implementation of the IOTA ipkcpd server, the poll() function was chosen as an efficient way to manage multiple client connections.

With poll(), the server can efficiently manage multiple client connections without the need for multiple threads or processes.

To use poll() in this context, the server initializes a list of file descriptors that includes the listening socket and any connected client sockets. It then enters a loop that calls poll() with the list of file descriptors and a timeout value set -1 designed to never time out.By setting the timeout value to -1, the server will block indefinitely until an event occurs on one of the file descriptors in the list. This approach ensures that the server can handle client connections in a responsive and efficient manner without wasting system resources by repeatedly checking for events when there are none.

If poll() returns a positive value, it indicates that there are file descriptors with events pending. The server then checks each file descriptor to determine whether it is the listening socket or a client socket. If it is the listening socket, the server accepts a new client connection and adds its file descriptor to the list. If a client socket has an event pending, the server will handle the data received by parsing and computing it. Once the computation is complete, the server will send the resulting data back to the client through the same socket.

This approach allows the IOTA ipkcpd server to efficiently manage multiple client connections while minimizing resource usage. By waiting on multiple file descriptors at once, the server can be more responsive and handle more clients simultaneously.

### Testing section
During the development process, testing was mainly performed manually. The majority of the tests focused on the lexer and parser modules, which are essential for computing and are used for both the binary and textual variants.

The tests covered a range of scenarios, including handling multiple client connections, parsing input data, and computing expressions accurately. Additionally, tests from the first assignment were also utilized to ensure the server's overall functionality.

The testing environment was on a local Arch-based machine and on a Nix OS virtual machine provided for testing purposes.

### Description of extra features
In addition to the main functionalities, an extra class called Frac was designed for intermediate calculations to be computed as fractions. This allows for more precise computations and provides a foundation for future implementation of sending data to clients in the representation of a number as a fraction.

The Frac class can be used to store rational numbers as a pair of integers (numerator and denominator) and perform arithmetic operations on them using simple operator overloading.


### Bibliography

<a id="1">[1]</a> Ing. Daniel Dolejška, Ing. Michal Koutenský,  _Programování síťových aplikací IPK 2022/2023 L_ , \[online\]

<a id="2">[2]</a> _Using poll() instead of select()_, last updated 8/31/2021 ,\[online\],https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select

<a id="3">[3]</a> _TCP Server-Client implementation in C_, Last Updated : 18 Nov, 2022 ,\[online\],https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

<a id="4">[4]</a> _Handle multiple socket connections with fd_set and select on Linux_, last updated July 31, 2020 ,\[online\],https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/

<a id="5">[5]</a> _TCP/IP Ports and Female Explained_, last updated April 14, 2021 ,\[online\],https://amcomen.org/c-server-client-send-receive-on-same-socket