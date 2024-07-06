#include "Cookie.h"
Cookie* Cookie::instance = nullptr;

Cookie *Cookie::getInstance() {
    if (!instance){
        instance = new Cookie;
    }
    return Cookie::instance;
}
