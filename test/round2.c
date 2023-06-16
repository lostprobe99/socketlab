/**
 * round2.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 17:11
 * @brief  : round2.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "util.h"

int main(int argc, char **argv)
{
    EXPECT(round_two(0), 1);
    EXPECT(round_two(1), 2);
    EXPECT(round_two(2), 2);
    EXPECT(round_two(3), 4);
    EXPECT(round_two(4), 4);
    EXPECT(round_two(5), 8);
    EXPECT(round_two(6), 8);
    EXPECT(round_two(7), 8);
    EXPECT(round_two(8), 8);
    EXPECT(round_two(9), 16);
    EXPECT(round_two(10), 16);
    EXPECT(round_two(11), 16);
    EXPECT(round_two(12), 16);
    EXPECT(round_two(13), 16);
    EXPECT(round_two(14), 16);
    EXPECT(round_two(15), 16);
    EXPECT(round_two(16), 16);

    return 0;
}