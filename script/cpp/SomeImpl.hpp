#include "NotAlgoCppInterface.hpp"

class SomeImpl : public NotAlgoCpp {
public:
    virtual int process(unsigned char image[], int width, int height, int type);
};
