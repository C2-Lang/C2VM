#include "args.h"

int ck_args(int argc, char *argv[])
{
    if(strcmp(argv[1], "--version") == 0)
    {
        printf("\x1b[32mC2VM: %s\x1b[0m\n", c2vmVer());
        printf("Made with love by Darek");
        return HAS_ARGS;
    }

    if(strcmp(argv[1], "--help") == 0)
    {
        printf("\x1b[32mC2VM Args commands\x1b[0m\n");
        /* There is no commands yet in the C2VM compiler */
        printf("No commands..");
        return HAS_ARGS;
    }
    

    return NO_ARGS;
}
