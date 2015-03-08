#include "SomeImpl.hpp"

NotAlgoCpp* getAlgo()
{
    NotAlgoCpp* algo = static_cast<NotAlgoCpp*>(new SomeImpl());
    return algo;
}

int SomeImpl::process(unsigned char image[], int width, int height, int type)
{
    return 42;
}
