#ifndef NODE_H
#define NODE_H

#include <queue>
using namespace std;

class Sec {
public:
    char *s[6];
    Sec()
    {
        for(int i = 0; i<6; ++i)
            s[i] = NULL;
    }

    ~Sec()
    {
        for(int i = 0; i<6; ++i)
            if(s[i] != NULL)
                delete(s[i]);
    }
};

struct SecList {
    queue<Sec *> q;
};

#endif