#include "all.h"

void day1(cstr in)
{
    dyn<int> cal;
    split(in, "\n"_s, [&](auto it)
    {
        int val = 0;
        for(umm i = 0; i < size(it); i++)
        {
            char c = it[i];
            if(c >= '0' && c <= '9')
            {
                val = 10 * (c - '0');
                break;
            }
        }
        for(umm i = 0; i < size(it); i++)
        {
            char c = it[size(it) - 1 - i];
            if(c >= '0' && c <= '9')
            {
                val += c - '0';
                break;
            }
        }
        cal.add(val);
    });
    print("Calibration values sum: %\n", sum(cal));

    dyn digits("one"_s, "two"_s, "three"_s, "four"_s, "five"_s, "six"_s, "seven"_s, "eight"_s, "nine"_s);
    auto dig = [&](cstr x)
    {
        for(umm i = 0; i < size(digits); i++)
        {
            if(size(x) >= size(digits[i]))
            {
                if(slice(x, 0, size(digits[i])) == digits[i])
                {
                    return int(i) + 1;
                }
            }
        }
        return 0;
    };
    cal.resize(0);
    split(in, "\n"_s, [&](auto it)
    {
        if(size(in) == 0) return;
        int val = 0;
        for(umm i = 0; i < size(it); i++)
        {
            char c = it[i];
            if(c >= '0' && c <= '9')
            {
                val = 10 * (c - '0');
                break;
            }
            int t = dig(slice(it, i));
            if(t)
            {
                val = 10 * t;
                break;
            }
        }
        for(umm i = 0; i < size(it); i++)
        {
            char c = it[size(it) - 1 - i];
            if(c >= '0' && c <= '9')
            {
                val += c - '0';
                break;
            }
            int t = dig(slice(it, size(it) - 1 - i));
            if(t)
            {
                val += t;
                break;
            }
        }
        cal.add(val);
    });
    print("Second calibration values sum: %\n", sum(cal));
}

int main()
{
    try
    {
        dstr in = filestr("day1.txt"_s);
        day1(in);
    }
    catch(const error& e)
    {
        print("Unhandled error: %\n", e.what());
    }
    return 0;
}
