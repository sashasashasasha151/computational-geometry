#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <list>

using namespace std;

class Point {
public:
    long long  x;
    long long  y;
    int  k;
    Point *prev = nullptr;
    Point *next = nullptr;

    Point(long long  x, long long  y, int  k) : x(x), y(y), k(k) {}
};

int n;

Point *start;
Point *cur;

int l_turn(Point a, Point b, Point c) {
    long long  f = (c.x - a.x) * (b.y - a.y);
    long long  s = (c.y - a.y) * (b.x - a.x);
    if (f - s < 0) {
        return 1;
    } else if (f - s > 0) {
        return -1;
    } else {
        return 0;
    }
}

bool in_t(Point d, Point a, Point b, Point c) {
    long long  x, y, k, m, n;
    long long  x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y, x3 = c.x, y3 = c.y;
    x = d.x;
    y = d.y;
    k = (x1 - x) * (y2 - y1) - (x2 - x1) * (y1 - y);
    m = (x2 - x) * (y3 - y2) - (x3 - x2) * (y2 - y);
    n = (x3 - x) * (y1 - y3) - (x1 - x3) * (y3 - y);
    return (k >= 0 && m >= 0 && n >= 0) || (k <= 0 && m <= 0 && n <= 0);
}

int main() {
    cin >> n;

    int x, y;
    cin >> x >> y;
    start = new Point(x, y, 1);
    cur = start;

    for (int i = 2; i <= n; ++i) {
        cin >> x >> y;
        auto *p = new Point(x, y, i);
        cur->next = p;
        p->prev = cur;
        cur = p;
    }

    cur->next = start;
    start->prev = cur;

    cur = start;

    while (n != 3) {
        if (l_turn(*cur->prev, *cur, *cur->next) == 1) {
            bool ear = true;

            Point *p = cur->next->next;
            for (int i = 0; i < n - 3; ++i) {
                if (in_t(*p, *cur->prev, *cur, *cur->next)) {
                    ear = false;
                    break;
                }
                p = p->next;
            }

            if (ear) {
                cout << cur->k << " " << cur->prev->k << " " << cur->next->k << "\n";
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                n--;
                cur = cur->prev;
                continue;
            }
        }
        cur = cur->next;
    }

    cout << cur->prev->k << " " << cur->k << " " << cur->next->k << "\n";

    return 0;
}
