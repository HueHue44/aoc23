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

void day3(cstr in)
{
    auto engine = split(in, "\n"_s);
    using point = sta<int, 2>;
    map<point, dyn<s64>> gears;
    s64 sum = 0;
    for(umm row = 0; row < size(engine); row++)
    {
        s64 num = 0;
        bool adj = false;
        dyn<point> stars;
        for(umm col = 0; col < size(engine[row]); col++)
        {
            char c = engine[row][col];
            if(c < '0' || c > '9')
            {
                if(num && adj)
                {
                    sum += num;
                }
                for(umm i = 0; i < size(stars); i++)
                {
                    gears[stars[i]].add(num);
                }
                num = 0;
                adj = false;
                stars.resize(0);
            }
            else
            {
                num = num * 10 + (c - '0');
                for(umm i = 0; i < 3; i++)
                {
                    for(umm j = 0; j < 3; j++)
                    {
                        if(row + i && col + j && col + j - 1 < size(engine[row + i - 1]))
                        {
                            char cadj = engine[row + i - 1][col + j - 1];
                            adj |= (cadj < '0' || cadj > '9') && cadj != '.';
                            point p{int(row + i - 1), int(col + j - 1)};
                            if(cadj == '*' && find(stars, p) == -1) stars.add(p);
                        }
                    }
                }
            }
        }
        if(num && adj)
        {
            sum += num;
        }
        for(umm i = 0; i < size(stars); i++)
        {
            gears[stars[i]].add(num);
        }
        num = 0;
        adj = false;
        stars.resize(0);
    }
    print("sum: %\n", sum);
    sum = 0;
    each(gears, [&](auto it)
    {
        const auto& vals = get<1>(it);
        if(size(vals) == 2) sum += vals[0] * vals[1];
    });
    print("sum of gear ratios: %\n", sum);
}

void day4(cstr in)
{
    s64 sum = 0;
    dyn<s64> wins;
    split(in, "\n"_s, [&](auto it)
    {
        if(size(it))
        {
            auto card = split(it, ": ");
            auto nums = split(card[1], " | ");
            auto win = split(nums[0], " ");
            for(umm i = 0; i < size(win); i++) if(!size(win[i])) win.remove(i--);
            auto have = split(nums[1], " ");
            for(umm i = 0; i < size(have); i++) if(!size(have[i])) have.remove(i--);
            s64 c = 0;
            for(umm i = 0; i < size(win); i++) c += s64(count(have, win[i]));
            wins.add(c);
            c = c ? (1 << (c - 1)) : 0;
            sum += c;
        }
    });
    print("sum: %\n", sum);
    dyn<s64> copies;
    for(umm i = 0; i < size(wins); i++) copies.add(s64(1));
    sum = 0;
    for(umm i = 0; i < size(wins); i++)
    {
        sum += wins[i] * copies[i] + 1;
        for(s64 w = 0; w < wins[i]; w++)
        {
            copies[i + umm(w) + 1] += copies[i];
        }
    }
    print("copies: %\n", sum);
}

int main()
{
    try
    {
        dstr in = filestr("day4.txt"_s);
        day4(in);
    }
    catch(const error& e)
    {
        print("Unhandled error: %\n", e.what());
    }
    return 0;
}
