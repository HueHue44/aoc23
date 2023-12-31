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
                            if(cadj == '*' && find(stars, p) == umm(-1)) stars.add(p);
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

void day7(cstr in)
{
    auto type = [&](const auto& cards)
    {
        s64 res = 0;
        for(s64 i = 2; i <= 14; i++)
        {
            if(count(cards, i) == 5) res = max(res, s64(6));
            if(count(cards, i) == 4) res = max(res, s64(5));
            for(s64 j = 2; j <= 14; j++) if(i != j && count(cards, i) == 3 && count(cards, j) == 2) res = max(res, s64(4));
            if(count(cards, i) == 3) res = max(res, s64(3));
            for(s64 j = 2; j <= 14; j++) if(i != j && count(cards, i) == 2 && count(cards, j) == 2) res = max(res, s64(2));
            if(count(cards, i) == 2) res = max(res, s64(1));
        }
        return res;
    };
    auto sorter = [&](const auto& a, const auto& b)
    {
        auto diff = type(get<0>(a)) - type(get<0>(b));
        if(diff < 0) return true;
        else if(diff > 0) return false;
        else
        {
            for(umm i = 0; i < size(get<0>(a)); i++)
            {
                diff = get<0>(a)[i] - get<0>(b)[i];
                if(diff < 0) return true;
                else if(diff > 0) return false;
            }
        }
        return true;
    };
    dyn<tup<dyn<s64>, s64>> hands;
    split(in, "\n"_s, [&](auto it)
    {
        if(!size(it)) return;
        auto parts = split(it, " "_s);
        dyn<s64> cards;
        for(umm i = 0; i < size(parts[0]); i++)
        {
            switch(parts[0][i])
            {
                case 'A': cards.add(s64(14)); break;
                case 'K': cards.add(s64(13)); break;
                case 'Q': cards.add(s64(12)); break;
                case 'J': cards.add(s64(11)); break;
                case 'T': cards.add(s64(10)); break;
                default: cards.add(s64(parts[0][i] - '0')); break;
            }
        }
        auto bid = toint(parts[1]);
        hands.add(tup(cards, bid));
    });
    hsort(hands, sorter);
    s64 sum = 0;
    for(umm i = 0; i < size(hands); i++) sum += get<1>(hands[i]) * s64(i + 1);
    print("winnings: %\n", sum);
    for(umm i = 0; i < size(hands); i++)
    {
        auto& cards = get<0>(hands[i]);
        for(umm j = 0; j < size(cards); j++) if(cards[j] == 11) cards[j] = 1;
    }
    auto sorter2 = [&](const auto& a, const auto& b)
    {
        auto ba = a;
        auto bb = b;
        for(s64 val = 1; val <= 14; val++)
        {
            auto tba = a;
            for(umm i = 0; i < size(get<0>(tba)); i++) if(get<0>(tba)[i] == 1) get<0>(tba)[i] = val;
            auto tbb = b;
            for(umm i = 0; i < size(get<0>(tbb)); i++) if(get<0>(tbb)[i] == 1) get<0>(tbb)[i] = val;
            if(type(get<0>(tba)) > type(get<0>(ba))) ba = tba;
            if(type(get<0>(tbb)) > type(get<0>(bb))) bb = tbb;
        }
        auto diff = type(get<0>(ba)) - type(get<0>(bb));
        if(diff < 0) return true;
        else if(diff > 0) return false;
        else
        {
            for(umm i = 0; i < size(get<0>(a)); i++)
            {
                diff = get<0>(a)[i] - get<0>(b)[i];
                if(diff < 0) return true;
                else if(diff > 0) return false;
            }
        }
        return true;
    };
    hsort(hands, sorter2);
    sum = 0;
    for(umm i = 0; i < size(hands); i++) sum += get<1>(hands[i]) * s64(i + 1);
    print("winnings2: %\n", sum);
}

void day8(cstr in)
{
    auto parts = split(in, "\n\n"_s);
    auto steps = parts[0];
    map<dstr, tup<dstr, dstr>> maps;
    split(parts[1], "\n"_s, [&](auto it)
    {
        if(!size(it)) return;
        dstr at = slice(it, 0, 3);
        dstr left = slice(it, 7, 3);
        dstr right = slice(it, 12, 3);
        maps[at] = tup(left, right);
    });
    dstr at = "AAA"_s;
    umm step = 0;
    while(at != "ZZZ"_s)
    {
        auto lr = steps[step % size(steps)];
        if(lr == 'L') at = get<0>(maps[at]);
        else at = get<1>(maps[at]);
        step++;
    }
    print("steps: %\n", step);
    dyn<dstr> ats;
    each(maps, [&](const auto& it) { if(get<0>(it)[2] == 'A') ats.add(get<0>(it)); });
    dyn<umm> cycles;
    cycles.resize(size(ats));
    step = 0;
    while(true)
    {
        for(umm i = 0; i < size(ats); i++)
        {
            auto& it = ats[i];
            auto lr = steps[step % size(steps)];
            if(lr == 'L') it = get<0>(maps[it]);
            else it = get<1>(maps[it]);
            if(!cycles[i] && it[2] == 'Z') cycles[i] = step + 1;
        }
        if(!count(cycles, umm(0))) break;
        step++;
    }
    umm factor = 1;
    while(factor <= min(cycles))
    {
        umm dc = 0;
        for(umm i = 0; i < size(cycles); i++) dc += (cycles[i] % factor == 0);
        if(dc == size(cycles)) cycles /= factor;
        factor++;
    }
    print("steps2: %\n", product(cycles) * size(steps));
}

void day9(cstr in)
{
    auto rec = [&](auto f, dyn<s64> x)
    {
        if(count(x, s64(0)) == size(x)) return sta(s64(0), s64(0));
        dyn<s64> diff;
        for(umm i = 1; i < size(x); i++) diff.add(x[i] - x[i - 1]);
        auto val = f(f, diff);
        auto first = x[0];
        auto last = x[size(x) - 1];
        return sta(first - val[0], val[1] + last);
    };
    sta<s64, 2> sum;
    split(in, "\n"_s, [&](auto it)
    {
        if(!size(it)) return;
        auto seq = split(it, " "_s);
        dyn<s64> nums;
        for(umm i = 0; i < size(seq); i++) nums.add(toint(seq[i]));
        sum += rec(rec, nums);
    });
    print("sum: %\n", sum);
}

void day10(cstr in)
{
    using point = sta<s64, 2>;
    map<point, char> pipes;
    auto lines = split(in, "\n"_s);
    point start;
    for(umm i = 0; i < size(lines); i++)
    {
        for(umm j = 0; j < size(lines[i]); j++)
        {
            char c = lines[i][j];
            point p{s64(i), s64(j)};
            pipes[p] = c;
            if(c == 'S') start = p;
        }
    }
    map<char, dyn<point>> dir;
    dir['|'].add(point{-1, 0}); dir['|'].add(point{1, 0});
    dir['-'].add(point{0, -1}); dir['-'].add(point{0, 1});
    dir['L'].add(point{-1, 0}); dir['L'].add(point{0, 1});
    dir['J'].add(point{-1, 0}); dir['J'].add(point{0, -1});
    dir['7'].add(point{1, 0});  dir['7'].add(point{0, -1});
    dir['F'].add(point{1, 0});  dir['F'].add(point{0, 1});
    dir['S'].add(point{-1, 0}); dir['S'].add(point{1, 0});
    dir['S'].add(point{0, -1}); dir['S'].add(point{0, 1});
    s64 steps = 0;
    point at = start;
    auto copy = pipes;
    while(true)
    {
        char c = copy[at];
        copy[at] = '#';
        s64 now = steps;
        each(dir[c], [&](auto d)
        {
            char n = copy[at + d];
            if(count(dir[n], -d) > 0 && now == steps)
            {
                at += d;
                steps++;
            }
        });
        if(now == steps) break;
    }
    print("steps: %\n", (steps + 1) / 2);
    s64 area = 0;
    for(umm i = 0; i < size(lines); i++)
    {
        s64 odd = 0;
        s64 dir = 0;
        for(umm j = 0; j < size(lines[i]); j++)
        {
            point p{s64(i), s64(j)};
            if(copy[p] == '#')
            {
                if(lines[i][j] != '-')
                {
                    s64 next = 0;
                    if(lines[i][j] == 'F' || lines[i][j] == '7') next = 1;
                    if(lines[i][j] == 'L' || lines[i][j] == 'J') next = -1;
                    if(dir == 0 || next == 0 || dir == next) odd ^= 1;
                    dir = next;
                    if(lines[i][j] == '7' || lines[i][j] == 'J') dir = 0;
                }
            }
            area += (copy[p] != '#' && odd);
        }
    }
    print("area: %\n", area);
}

void day11(cstr in)
{
    auto rows = split(in, "\n"_s);
    rows.remove(size(rows) - 1);
    dyn<s64> er;
    dyn<s64> ec;
    for(umm i = 0; i < size(rows); i++) if(count(rows[i], '.') == size(rows[i])) er.add(s64(i));
    for(umm i = 0; i < size(rows[0]); i++)
    {
        umm j = 0;
        for(; j < size(rows); j++) if(rows[j][i] != '.') break;
        if(j == size(rows)) ec.add(s64(i));
    }
    using p2 = sta<s64, 2>;
    dyn<p2> g;
    for(umm r = 0; r < size(rows); r++)
    {
        for(umm c = 0; c < size(rows[r]); c++)
        {
            p2 p{s64(r), s64(c)};
            if(rows[r][c] == '#') g.add(p);
        }
    }
    auto taxi = [&](auto x, auto y, s64 adder = 1)
    {
        auto d = x - y;
        d[0] = abs(d[0]);
        d[1] = abs(d[1]);
        each(er, [&](auto it){ if(it > min(x[0], y[0]) && it < max(x[0], y[0])) d[0] += adder; });
        each(ec, [&](auto it){ if(it > min(x[1], y[1]) && it < max(x[1], y[1])) d[1] += adder; });
        s64 res = d[0] + d[1];
        return res;
    };
    s64 tot = 0;
    for(umm i = 0; i < size(g); i++)
    {
        for(umm j = i + 1; j < size(g); j++)
        {
            tot += taxi(g[i], g[j]);
        }
    }
    print("sum: %\n", tot);
    tot = 0;
    for(umm i = 0; i < size(g); i++)
    {
        for(umm j = i + 1; j < size(g); j++)
        {
            tot += taxi(g[i], g[j], 999999);
        }
    }
    print("sum2: %\n", tot);
}

void day12(cstr in)
{
    map<tup<dstr, dyn<s64>, s64>, s64> mem;
    auto comb = [&](auto f, cstr x, dyn<s64> ord, s64 c = 0) -> s64
    {
        if(size(x) < sum(ord) - c) return 0;
        auto check = mem.get(tup(dstr(x), ord, c));
        if(check.valid()) return get<1>(check.value());
        for(umm i = 0; i < size(x); i++)
        {
            if(x[i] == '#')
            {
                c++;
                if(!size(ord) || c > ord[0]) return 0;
            }
            else if(x[i] == '?')
            {
                dstr n = slice(x, i);
                n[0] = '.';
                s64 a = f(f, n, ord, c);
                mem[tup(n, ord, c)] = a;
                n[0] = '#';
                s64 b = f(f, n, ord, c);
                mem[tup(n, ord, c)] = b;
                return a + b;
            }
            else if(x[i] == '.')
            {
                if(c)
                {
                    if(!size(ord) || c != ord[0]) return 0;
                    ord.remove(0);
                    c = 0;
                }
            }
        }
        if(c && size(ord) == 1 && c == ord[0]) return 1;
        return !size(ord) && !c;
    };
    auto test = [&](auto in)
    {
        auto parts = split(in, " "_s);
        dyn<s64> ord;
        split(parts[1], ","_s, [&](auto it){ ord.add(toint(it)); });
        return comb(comb, parts[0], ord);
    };
    s64 sum = 0;
    split(in, "\n"_s, [&](auto it)
    {
        if(!size(it)) return;
        auto parts = split(it, " "_s);
        dyn<s64> ord;
        split(parts[1], ","_s, [&](auto it){ ord.add(toint(it)); });
        sum += comb(comb, parts[0], ord);
    });
    print("sum: %\n", sum);
    sum = 0;
    split(in, "\n"_s, [&](auto it)
    {
        if(!size(it)) return;
        auto parts = split(it, " "_s);
        dstr a = parts[0];
        dstr b = parts[1];
        dstr ta = a;
        ta.add('?');
        dstr tb = b;
        tb.add(',');
        for(umm i = 0; i < 4; i++)
        {
            a.insert(0, ta);
            b.insert(0, tb);
        }
        dyn<s64> ord;
        split(b, ","_s, [&](auto it){ ord.add(toint(it)); });
        sum += comb(comb, a, ord);
    });
    print("sum2: %\n", sum);
}

void day13(cstr in)
{
    auto test = [&](auto x)
    {
        auto lines = split(x, "\n"_s);
        if(!size(lines[size(lines) - 1])) lines.remove(size(lines) - 1);
        dyn<s64> all;
        umm col = 1;
        while(col < size(lines[0]))
        {
            bool ok = true;
            for(umm i = 0; col >= 1 + i && col + i < size(lines[0]) && ok; i++)
            {
                for(umm j = 0; j < size(lines) && ok; j++)
                {
                    if(lines[j][col - 1 - i] != lines[j][col + i]) ok = false;
                }
            }
            if(ok) all.add(s64(col));
            col++;
        }
        umm row = 1;
        while(row < size(lines))
        {
            bool ok = true;
            for(umm i = 0; row >= 1 + i && row + i < size(lines) && ok; i++)
            {
                if(lines[row - 1 - i] != lines[row + i]) ok = false;
            }
            if(ok) all.add(s64(row) * 100);
            row++;
        }
        return all;
    };
    s64 sum = 0;
    split(in, "\n\n"_s, [&](auto pat)
    {
        if(!size(pat)) return;
        auto r = ::sum(test(pat));
        sum += r;
    });
    print("sum: %\n", sum);
    sum = 0;
    split(in, "\n\n"_s, [&](auto pat)
    {
        if(!size(pat)) return;
        dstr fix = pat;
        auto def = ::sum(test(fix));
        for(umm i = 0; i < size(fix); i++)
        {
            if(fix[i] == '.')
            {
                fix[i] = '#';
                auto r = test(fix);
                for(umm j = 0; j < size(r); j++)
                {
                    if(r[j] != def)
                    {
                        sum += r[j];
                        return;
                    }
                }
                fix[i] = '.';
            }
            else if(fix[i] == '#')
            {
                fix[i] = '.';
                auto r = test(fix);
                for(umm j = 0; j < size(r); j++)
                {
                    if(r[j] != def)
                    {
                        sum += r[j];
                        return;
                    }
                }
                fix[i] = '#';
            }
        }
    });
    print("sum2: %\n", sum);
}

int main()
{
    try
    {
        dstr in = filestr("day13.txt"_s);
        day13(in);
    }
    catch(const error& e)
    {
        print("Unhandled error: %\n", e.what());
    }
    return 0;
}
