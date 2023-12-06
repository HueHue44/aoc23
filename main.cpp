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

void day5(cstr in)
{
    auto maps = split(in, "\n\n");
    auto first = split(maps[0], "seeds: ")[1];
    s64 lowest = 1LL << 60LL;
    s64 lowest2 = lowest;
    s64 prev = -1;
    split(first, " "_s, [&](auto it)
    {
        auto seed = toint(it);
        dyn<sta<s64, 2>> track;
        if(prev == -1) prev = seed;
        else track.add(sta(prev, seed));
        for(umm i = 1; i < size(maps); i++)
        {
            auto next = seed;
            dyn<sta<s64, 2>> mapped;
            split(split(maps[i], ":\n")[1], "\n"_s, [&](auto range)
            {
                if(!size(range)) return;
                auto parts = split(range, " ");
                auto dst = toint(parts[0]);
                auto src = toint(parts[1]);
                auto count = toint(parts[2]);
                if(seed >= src && seed <= src + count) next = dst + (seed - src);
                for(umm ti = 0; ti < size(track); ti++)
                {
                    auto r = track[ti];
                    s64 left = max(src, r[0]);
                    s64 right = min(src + count, r[0] + r[1]);
                    if(left <= right)
                    {
                        track.remove(ti--);
                        mapped.add(sta(dst + (left - src), right - left));
                        if(r[0] < src) track.add(sta(r[0], src - r[0] - 1));
                        if(r[0] + r[1] > src + count) track.add(sta(src + count + 1, r[0] + r[1] - (src + count + 1)));
                    }
                }
            });
            track.add(mapped);
            seed = next;
        }
        lowest = min(lowest, seed);
        for(umm ti = 0; ti < size(track); ti++) lowest2 = min(lowest2, track[ti][0]);
        if(size(track)) prev = -1;
    });
    print("lowest: %\n", lowest);
    print("lowest2: %\n", lowest2);
}

void day6(cstr in)
{
    auto lines = split(in, "\n");
    auto times = split(lines[0], " ");
    times.remove(0);
    for(umm i = 0; i < size(times); i++) if(!size(times[i])) times.remove(i--);
    auto dists = split(lines[1], " ");
    dists.remove(0);
    for(umm i = 0; i < size(dists); i++) if(!size(dists[i])) dists.remove(i--);
    dyn<s64> wins;
    wins.resize(size(times));
    for(umm i = 0; i < size(times); i++)
    {
        auto maxt = toint(times[i]);
        auto record = toint(dists[i]);
        for(s64 time = 0; time < maxt; time++)
        {
            auto dist = time * (maxt - time);
            if(dist > record) wins[i]++;
        }
    }
    print("result: %\n", product(wins));
    dstr total;
    for(umm i = 0; i < size(times); i++) total.add(times[i]);
    auto time = toint(total);
    total.resize(0);
    for(umm i = 0; i < size(dists); i++) total.add(dists[i]);
    auto dist = toint(total);
    s64 num = 0;
    for(s64 t = 0; t < time; t++) num += t * (time - t) > dist;
    print("result2: %\n", num);
}

int main()
{
    try
    {
        dstr in = filestr("day6.txt"_s);
        day6(in);
    }
    catch(const error& e)
    {
        print("Unhandled error: %\n", e.what());
    }
    return 0;
}
