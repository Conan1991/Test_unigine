#ifndef SINGLETON_H
#define SINGLETON_H

#include "Qt/qmutex.h"
#include <cassert>


template <typename T>
class Singleton
{
public:  
    static T* instance() {
        QMutex m;
        QMutexLocker locker(&m);
        if (_self == 0)
            _self = new T;
        assert(_self != 0);
        return _self;
    };
    
   static void resetInstance() {
      delete _self; // REM : it works even if the pointer is NULL (does nothing then)
      _self = nullptr; // so GetInstance will still work.
   }

protected:

    Singleton() {
        /***/ 
    };

    virtual ~Singleton() {
		_self = 0; /***/
	};

protected:

    Singleton(const Singleton& source){
        /***/ 
    };

    static T* _self;
};

template < typename T > T* Singleton<T>::_self = 0;


#endif // SINGLETON_H