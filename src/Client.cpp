#include "Client.hpp"

Client::Client(int fd): fd(fd) {
    isOperator = false;
    state = UNAUTHENTICATED;
}
