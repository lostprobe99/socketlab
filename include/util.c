/**
 * util.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.c
 */
int round_two(int x)
{
    if (x == 0)
        return 1;
    if (x == 1)
        return 2;
    // x 是 2 的幂
    if ((x & (x - 1)) == 0)
        return x;
    int last;
    do
    {
        // 不断循环，保留最高位的 1
        last = x;
        x = x & (x - 1);
    } while (x != 0);

    // 最后左移 1
    return last << 1;
}
