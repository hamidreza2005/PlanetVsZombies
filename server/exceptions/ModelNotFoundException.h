#ifndef SERVER_MODELNOTFOUNDEXCEPTION_H
#define SERVER_MODELNOTFOUNDEXCEPTION_H
#include "exception"
#include "QString"
class ModelNotFoundException : public std::exception
{
    QString msg;
public:
    ModelNotFoundException(QString msg);

    const char* what() const noexcept override;
};


#endif //SERVER_MODELNOTFOUNDEXCEPTION_H
