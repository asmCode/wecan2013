#pragma once

#include <stddef.h>

template <typename T>
class GenericSingleton
{
protected:
    static T *instance;

public:
    static T* GetInstance()
    {
        if (instance == NULL)
            instance = new T();

        return instance;
    }

    static void DestroyInstance()
    {
        if (instance != NULL)
        {
            delete instance;
            instance = NULL;
        }
    }
};