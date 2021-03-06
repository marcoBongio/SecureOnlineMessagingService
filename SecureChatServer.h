#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <openssl/evp.h>
#include <vector>
#include <thread>
#include "User.h"

class SecureChatServer{
    private:
        //Server private key
        static EVP_PKEY* server_prvkey;

        //Server certificate
        static X509* server_certificate;

        //Port and listening IP address, in dotted notation (e.g. 192.168.1.1)
        int listening_socket;
        char address[MAX_ADDRESS_SIZE];
        unsigned short int port;
        struct sockaddr_in server_addr;

        //Get the server private key
        static EVP_PKEY* getPrvKey();

        //Get the specified user private key
        static EVP_PKEY* getUserKey(string username);

        //Get the server certificate
        static X509* getCertificate();

        //Setup the socket
        void setupSocket();

        //Let the main process listen to client requests
        void listenRequests();

        //Send the certificate to a client
        void sendCertificate(int process_socket, unsigned char* R_server);

        //Receive authentication from user
        string receiveAuthentication(int process_socket, unsigned int &status, unsigned char* R_server, unsigned char* &R_user, EVP_PKEY* &tpubk);

        void handleConnection(int data_socket, sockaddr_in client_address);

        //Change user status
        void changeUserStatus(string username, unsigned int status, int socket);

        void printUserList();

        //Send the list of available users
        void sendAvailableUsers(int data_socket, string username);

        vector<User> getOnlineUsers();

        //Receive Request To Talk
        string receiveRTT(int data_socket, string username);

        //Forward a RTT to the final receiver
        void forwardRTT(string receiver_username, string sender_username);

        //Receive response to RTT
        string receiveResponse(int data_socket, string receiver_username, unsigned int &response);

        //Forward response to RTT
        void forwardResponse(string sender_username, string username, unsigned int response);

        //Send user public key to the users that want to communicate
        void sendUserPubKey(string username, int data_socket, string key_receiver);

        //Receive a refresh message
        bool checkRefresh(char* msg, unsigned int buffer_len, string username);

        //Send a message to the user to return him to lobby
        void sendBadResponse(int data_socket, string username);

        //Receive a logout message
        void checkLogout(int data_socket, int other_socket, char* msg, unsigned int buffer_len, string username, string other_username);

        void receive(int data_socket, string username, unsigned int &len, unsigned char* &msg, const unsigned int max_size);

        void forward(string username, unsigned char* msg, unsigned int len);

        void wait(string username);

        void notify(string username);

        void handleChat(int sender_socket, int receiver_socket, string sender, string receiver);

        void sendS3Message(int data_socket, unsigned char* K, unsigned char* R_user, EVP_PKEY* tpubk, unsigned char* &iv);

        void setCounters(unsigned char* iv, string username);

        void incrementCounter(int counter, string username);

        void checkCounter(int counter, string username, unsigned char* received_counter);

        void storeK(string username, unsigned char* K);

        void waitForAck(int data_socket, string username);

        void checkLobby(char* msg, unsigned int buffer_len, string username, int data_socket, string other_username);

    public:
        //Constructor that gets as inputs the address, the port and the user filename.
        SecureChatServer(const char* addr, unsigned short int port, const char *user_filename);

        //Destructor to close the listening socket
        ~SecureChatServer();

        //List of users
        static map<string, User> *users;
};