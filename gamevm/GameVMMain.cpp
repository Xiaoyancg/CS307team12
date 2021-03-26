#include <GameVM.h>
#include <Timer.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    Timer t = Timer();
    int r = GameVMMain(argc, argv);

    printf("t%f", t.getPassedTimeInSec());
    return r;
}
