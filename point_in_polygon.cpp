#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct Point {
    long long x;
    long long y;
};

struct Edge {
    Point a;
    Point b;
};

int n;
vector<Point> points;
vector<Edge> edges;

int l_turn(long long ax, long long ay, long long bx, long long by, long long cx, long long cy) {
    double f = (cx - ax) * (by - ay);
    double s = (cy - ay) * (bx - ax);
    double ans = f - s;

    if (ans > 0) {
        return 1;
    }

    if (ans < 0) {
        return -1;
    }

    return 0;
}

bool is_x(Edge a, Edge b) {
    long long x1, y1, x2, y2, x3, y3, x4, y4;
    x1 = a.a.x;
    y1 = a.a.y;
    x2 = a.b.x;
    y2 = a.b.y;
    x3 = b.a.x;
    y3 = b.a.y;
    x4 = b.b.x;
    y4 = b.b.y;

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    if (x3 > x4) {
        swap(x3, x4);
        swap(y3, y4);
    }

    if (x1 > x3) {
        swap(x1, x3);
        swap(x2, x4);
        swap(y1, y3);
        swap(y2, y4);
    }

    if (x3 > x2) {
        return false;
    }

    if (y1 > y2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    if (y3 > y4) {
        swap(x3, x4);
        swap(y3, y4);
    }

    if (y1 > y3) {
        swap(x1, x3);
        swap(x2, x4);
        swap(y1, y3);
        swap(y2, y4);
    }

    if (y2 < y3) {
        return false;
    }

    int fl = l_turn(x1, y1, x2, y2, x3, y3);
    int fr = l_turn(x1, y1, x2, y2, x4, y4);
    int sl = l_turn(x3, y3, x4, y4, x1, y1);
    int sr = l_turn(x3, y3, x4, y4, x2, y2);

    if (abs(fl + fr) == 2 || abs(sl + sr) == 2) {
        return false;
    } else {
        return true;
    }
}

int main() {
    cin >> n;

    long long xc, yc;
    cin >> xc >> yc;

    long long x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        points.push_back({x, y});
        if (x == xc && y == yc) {
            cout << "YES";
            return 0;
        }
    }

    int l = 0;
    for (int i = 1; i < n; ++i) {
        while (i < n - 1 && points[i].x == points[i + 1].x && points[i].y == points[i + 1].y) {
            i++;
        }
        edges.push_back({points[l], points[i]});
        l = i;
    }

    if (!(points[0].x == points[0].y && points.back().x == points.back().y)) {
        edges.push_back({points[0], points.back()});
    }

    Edge check = {{xc,         yc},
                  {1000000001, yc + 1}};

    int ans = 0;

    for (Edge e : edges) {
        if (is_x(e, check)) {
            ans++;
        }
    }

    if (ans % 2 == 0) {
        cout << "NO";
    } else {
        cout << "YES";
    }


    return 0;
}
