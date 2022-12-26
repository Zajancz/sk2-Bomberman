#include "EventHandler.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <signal.h>
#include <errno.h>
#include <error.h>
#include <unordered_set>
#include "Client.h"

namespace Server {
    /// @brief ServerEventHandler is used for handling all new connections to the server
    class ServerEventHandler : public EventHandler {
        int * serverSocket;
        std::unordered_set<Client*> * clients;
    public:
        ServerEventHandler(int * serverSocketPtr, std::unordered_set<Client*> * clientsPtr);
        virtual void handleEventEpollin(uint32_t events) override;
        virtual void handleEventEpollout(uint32_t events) override;
        void ctrl_c(int);
    };
}