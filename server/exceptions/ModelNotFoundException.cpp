#include "ModelNotFoundException.h"

ModelNotFoundException::ModelNotFoundException(QString msg): std::exception(), msg(msg){

};

const char* ModelNotFoundException::what() const noexcept
{
    return msg.toStdString().c_str();
}