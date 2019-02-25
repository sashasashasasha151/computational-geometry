#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

struct Point {
    double x;
    double y;
};

struct Circle {
    Point p;
    double dim;
};

int n;
Point points[1000000];
std::random_device rnd;
std::mt19937 g(rnd());

double dist(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool point_in_circle(const Circle &c, const Point &p) {
    return dist(p, c.p) * 4 <= c.dim;
}

Circle make_circle(const Point &p1, const Point &p2, const Point &p3) {
    double offset = p2.x * p2.x + p2.y * p2.y;
    double bc = p1.x * p1.x + p1.y * p1.y - offset;
    double cd = offset - p3.x * p3.x - p3.y * p3.y;
    double det = (p1.x - p2.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p2.y);

    double idet = 0.5 / det;

    double centerx = (bc * (p2.y - p3.y) - cd * (p1.y - p2.y)) * idet;
    double centery = (cd * (p1.x - p2.x) - bc * (p2.x - p3.x)) * idet;
    double radius = (p2.x - centerx) * (p2.x - centerx) + (p2.y - centery) * (p2.y - centery);

    return {{centerx, centery}, radius * 4};
}

Circle make() {
    Circle st = {{(points[0].x + points[1].x) * 0.5, (points[0].y + points[1].y) * 0.5}, dist(points[0], points[1])};
    for (int i = 2; i < n; ++i) {
        if (!point_in_circle(st, points[i])) {
            st = {{(points[0].x + points[i].x) * 0.5, (points[0].y + points[i].y) * 0.5}, dist(points[0], points[i])};
            for (int j = 1; j < i; ++j) {
                if (!point_in_circle(st, points[j])) {
                    st = {{(points[j].x + points[i].x) * 0.5, (points[j].y + points[i].y) * 0.5}, dist(points[j], points[i])};
                    for (int ii = 0; ii < j; ++ii) {
                        if (!point_in_circle(st, points[ii])) {
                            st = make_circle(points[ii], points[j], points[i]);
                        }
                    }
                }
            }
        }
    }
    return st;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    cin >> n;
    double x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        points[i] = {x, y};
    }

    if (n == 1) {
        cout.precision(15);
        cout << 0.0 << "\n" << points[0].x << " " << points[0].y;
        return 0;
    }

    shuffle(points, points + n, g);
    Circle ans = make();

    cout.precision(15);
    cout << sqrt(ans.dim) * 0.5 << "\n" << ans.p.x << " " << ans.p.y;

    return 0;
}
/*
10
4645 65
7 567
567 5534
5 58
854 5
458 67
3557 3534
4534 34534
43234 32467
7654 34
 */
