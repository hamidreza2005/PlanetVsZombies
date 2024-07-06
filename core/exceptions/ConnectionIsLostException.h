#ifndef PLANETVSZOMBIES_CONNECTIONISLOSTEXCEPTION_H
#define PLANETVSZOMBIES_CONNECTIONISLOSTEXCEPTION_H
#include "exception"

class ConnectionIsLostException : public std::exception{
public:
    ConnectionIsLostException();
    const char* what() const noexcept override;
};


#endif //PLANETVSZOMBIES_CONNECTIONISLOSTEXCEPTION_H
