#include "wavheader.h"
#include "sinesound.h"
using namespace std;
int main(void)
{
    sinesound s(10, 1000, 44100);
    s.generatewav(0, "SineClassFile.wav");
    return 0;
}