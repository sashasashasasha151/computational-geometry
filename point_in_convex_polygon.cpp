#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    long long x, y;
};

bool operator<(const Point &a, const Point &b) {
    if ((a.y == 0 && b.y == 0) || (a.x == 0 && b.x == 0)) {
        return false;
    }

    if (a.y == 0 && b.x == 0) {
        return b.y > 0;
    }

    if (a.x == 0 && b.y == 0) {
        return a.y < 0;
    }

    long long f = a.y * b.x;
    long long s = b.y * a.x;

    if (f == s) {
        return false;
    } else {
        return f < s;
    }
}

bool cmp(const Point &a, const Point &b) {
    if (a.x == 0 && b.x == 0) {
        if ((a.y > 0 && b.y > 0) || (a.y < 0 && b.y < 0)) {
            return a.y > b.y;
        } else {
            return a.y < b.y;
        }
    }

    if (a.y == 0 && b.y == 0) {
        return a.x > b.x;
    }

    if (a.y == 0 && b.x == 0) {
        return b.y > 0;
    }

    if (a.x == 0 && b.y == 0) {
        return a.y < 0;
    }

    long long f = a.y * b.x;
    long long s = b.y * a.x;

    if (f == s) {
        if (a.y < 0) {
            return a.x < b.x;
        } else {
            return a.x > b.x;
        }
    } else {
        return f < s;
    }
}

long long l_turn(Point &a, Point &b, Point &c) {
    long long f = (c.x - a.x) * (b.y - a.y);
    long long s = (b.x - a.x) * (c.y - a.y);
    if (f - s < 0) {
        return -1;
    } else if (f - s > 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
//    int n;
//    cin >> n;
//
//    vector<Point> ptr;
//    int zi = 0;
//
//    long long x, y;
//    for (int i = 0; i < n; i++) {
//        cin >> x >> y;
//        ptr.push_back({x, y});
//        if (ptr[i].x < ptr[zi].x || (ptr[i].x == ptr[zi].x && ptr[i].y < ptr[zi].y)) {
//            zi = i;
//        }
//    }

    int n;
    cin >> n;

    vector<Point> ptr;
    int zi = 0;

    long long x, y;

    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        ptr.push_back({x, y});
        if (ptr[i].x < ptr[zi].x || (ptr[i].x == ptr[zi].x && ptr[i].y < ptr[zi].y)) {
            zi = i;
        }
    }

    Point z = ptr[zi];

    rotate(ptr.begin(), ptr.begin() + zi, ptr.end());
    ptr.erase(ptr.begin());

    for (int i = 0; i < n; i++) {
        ptr[i].x -= z.x;
        ptr[i].y -= z.y;
    }

    if (cmp(ptr[1], ptr[0])) {
        reverse(ptr.begin(), ptr.end());
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        Point point;
        cin >> point.x >> point.y;
//    for (int i = 0; i <= 10; ++i) {
//        cout << endl;
//        for (int j = 0; j <= 8; ++j) {
//
//            Point point = {i, j};
        point.x -= z.x;
        point.y -= z.y;

//        cout << i << " : " << j << " -- ";

        if (point.x < 0) {
            cout << "OUTSIDE\n";
            continue;
        }


        int ans = 0;

        if (point.x == 0 && point.y == 0) {
            cout << "BORDER\n";
            continue;
        }

        int idd = 0;
        while (!(ptr[idd] < ptr[idd + 1])) {
            idd++;
        }
//        idd--;
        Point tmpp = {0, 0};

        if (point.x >= 0 && point.y <= 0 && point.x <= ptr[idd].x && point.y >= ptr[idd].y &&
            l_turn(tmpp, ptr[idd], point) == 0) {
            cout << "BORDER\n";
            continue;
        }

        auto it = upper_bound(ptr.begin(), ptr.end(), point);
//        if (it != ptr.end()) {
//            cout << it.base()->x + z.x << " " << it.base()->y + z.y << " -- ";
//        } else {
//            cout << "end -- ";
//        }

        if (it == ptr.end()) {
            int id = ptr.size() - 1;
            while (!(ptr[id - 1] < ptr[id])) {
                id--;
            }

            Point tmp = {0, 0};
            if (l_turn(tmp, ptr[id], point) != 0) {
                cout << "OUTSIDE\n";
            } else {
                if (point.x <= ptr[id].x && (ptr[id].y <= 0 && point.y >= ptr[id].y || ptr[id].y >= 0 && point.y <= ptr[id].y)) {
                    cout << "BORDER\n";
                } else {
                    cout << "OUTSIDE\n";
                }
            }
            continue;
        }

        int ind = int(it - ptr.begin());

        int lt;
        Point tmp = {0, 0};

        if (ind == 0) {
            lt = l_turn(tmp, ptr[ind], point);
        } else {
            lt = l_turn(ptr[ind - 1], ptr[ind], point);
        }

        if (lt < 0) {
            ans = 2;
        } else if (lt == 0) {
            ans = 1;
        }

        if (ans == 0) {
            cout << "OUTSIDE\n";
        } else if (ans == 1) {
            cout << "BORDER\n";
        } else {
            cout << "INSIDE\n";
        }
//        }
    }
    return 0;
}

/*
8
5 0
8 0
10 2
8 6
0 9
0 6
0 5
2 2
10000
5 0
8 0
10 2
8 6
0 9
0 6
0 5
2 2

4
0 0
2 0
2 2
0 2
1
1 0
 */

/*
8
5 0
8 0
10 2
8 6
0 9
0 6
0 5
2 2
10000
5 0
8 0
10 2
8 6
0 9
0 6
0 5
2 2

4
0 0
2 0
2 2
0 2
1
1 0

3
97 24
-69 72
-28 -96

3
6 1
-3 4
-2 -5
 */
