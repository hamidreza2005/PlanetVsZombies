#include "ConnectionIsLostException.h"

const char *ConnectionIsLostException::what() const noexcept {
    return "Connection Is Lost";
}

ConnectionIsLostException::ConnectionIsLostException(): std::exception() {

}
