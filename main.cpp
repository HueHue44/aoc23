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

void day2(cstr in)
{
    map<dstr, s64> bag;
    bag["red"_s] = 12;
    bag["green"_s] = 13;
    bag["blue"_s] = 14;
    s64 sum = 0;
    s64 sum2 = 0;
    split(in, "\n"_s, [&](auto row)
    {
        if(size(row) == 0) return;
        bool possible = true;
        map<dstr, s64> minimum;
        auto game = split(row, ":"_s)[1];
        split(slice(game, 1), "; "_s, [&](auto round)
        {
            if(size(round) == 0) return;
            split(round, ", "_s, [&](auto cubes)
            {
                auto num = toint(split(cubes, " "_s)[0]);
                auto color = split(cubes, " "_s)[1];
                if(bag[color] < num) possible = false;
                minimum[color] = max(minimum[color], num);
            });
        });
        if(possible)
        {
            auto id = toint(split(split(row, ":"_s)[0], " "_s)[1]);
            sum += id;
        }
        sum2 += minimum["red"_s] * minimum["green"_s] * minimum["blue"_s];
    });
    print("Sum of IDs: %\n", sum);
    print("Sum of the power: %\n", sum2);
}

int main()
{
    try
    {
        dstr in = filestr("day2.txt"_s);
        day2(in);
    }
    catch(const error& e)
    {
        print("Unhandled error: %\n", e.what());
    }
    return 0;
}
