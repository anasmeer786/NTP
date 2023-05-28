
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        printf("Failed to connect to server");
        return -1;
    }

    char buffer[BUFFER_SIZE] = {0};

    // seed the random number generator with the current time
    srand(time(NULL));
    // generate a random clock drift rate between -50 and 50 milliseconds per second
    int clock_drift_rate = (rand() % 101) - 50;

    while (1)
    {
        // get the local time and add the clock drift error
        getLocalTime(buffer);
        long long lc = atoll(buffer);
        long long rc = 0;
        if (recv(sock, buffer, BUFFER_SIZE, 0) != -1)
        {
            rc = atoll(buffer);
        }
        long long r = (long long)(clock_drift_rate * (lc - rc) / 1000);
        lc += r;

        // send the adjusted local time to the server
        sprintf(buffer, "%lld", lc);
        if (send(sock, buffer, strlen(buffer), 0) == -1)
        {
            printf("Failed to send data to server");
            return -1;
        }

        printf("Sent time to server: %s\n", buffer);

        if (recv(sock, buffer, BUFFER_SIZE, 0) == -1)
        {
            printf("Failed to receive data from server");
            return -1;
        }

        printf("Received adjusted time from server: %s\n", buffer);

        usleep(100000);
    }

    close(sock);
    return 0;
}

