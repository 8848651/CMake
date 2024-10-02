
#pragma once

#include "stl/vector.h"

namespace tool {

    class ConnectionStruct {
    public:
        int cfd;
        //TODO: 以后将其换为vector不必担心长度问题
        stl::vector<int> client_id;
        ConnectionStruct() {};
        ConnectionStruct(int cfd, void* args) {
            //this->cfd = cfd;
            //this->client_id = (int(*)[10])args;
        };
    };

}