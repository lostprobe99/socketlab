#include <iostream>

class ARRAY
{
private:
    int n, a[100], pos, neg;
public:
    ARRAY(int x[], int size);
    void Num();
    void Show();
};

ARRAY::ARRAY(int x[], int size)
: pos(0), neg(0)
{
    n = size;
    for (int i = 0; i < n; i++)
        a[i] = x[i];
}

void ARRAY::Num()
{
    for(int i = 0; i < n; i++)
    {
        if(a[i] > 0)
            pos++;
        else
            neg++;
    }
}

void ARRAY::Show()
{
    for(int i = 0; i < n; i++)
    {
        ::printf("%5d", a[i]);
        if(i % 5 == 4)
            std::cout << std::endl;
    }
    ::printf("\npos = %d\n", pos);
    ::printf("neg = %d\n", neg);
}

int main()
{
    int b[] = {3, -4, 6, -8, 10, -34, 2, 90, -100};
    auto a = ARRAY(b, sizeof(b) / sizeof(b[0]));
    a.Num();
    a.Show();
    return 0;
}