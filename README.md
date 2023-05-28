# NTP
clock synchronization algorithm between a client and a server. 
The client adjusts its local time based on a random clock drift rate and exchanges time information with the server. 
The server sends timestamps to the client, allowing the client to synchronize its local time with the server's time.

The code you provided consists of two separate parts: a client-side code and a server-side code. Let's analyze each part separately:

Client-side code:

The client-side code establishes a connection to a server using sockets.
It generates a random clock drift rate between -50 and 50 milliseconds per second.
Inside an infinite loop, it retrieves the local time, adjusts it with the clock drift error, and sends it to the server.
It then receives the adjusted time from the server and repeats the process after a delay of 100 milliseconds.
The client-side code closes the socket connection when the loop is terminated.
Server-side code:

The server-side code initializes a mutex to synchronize access to shared resources.
It creates a socket and binds it to a specific address and port.
The server listens for client connections and accepts them.
Each client connection is handled in a separate thread.
Inside the thread, the server retrieves the current time, converts it to a timestamp, and sends it to the client every second.
The thread continues to send timestamps to the client until it is terminated.
Overall, the client-side code establishes a connection to the server and exchanges time-related information, while the server-side code listens for client connections and sends timestamps to the connected clients. 
