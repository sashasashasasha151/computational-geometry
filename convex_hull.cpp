#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

struct Point {
    long long x;
    long long y;
};

int n;
int zi = 0;
vector<Point> ptr;
vector<int> indexes;
vector<int> stack;

long long dist(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int l_turn(Point a, Point b, Point c) {
    long long f = (c.x - a.x) * (b.y - a.y);
    long long s = (c.y - a.y) * (b.x - a.x);
    if (f - s < 0) {
        return 1;
    } else if (f - s > 0) {
        return -1;
    } else {
        return 0;
    }
}

bool cmp(int l, int r) {
    Point a = ptr[l];
    Point b = ptr[r];

    if (a.x == 0 && b.x == 0) {
        if (a.y > 0 && b.y > 0 || (a.y < 0 && b.y < 0)) {
            return a.y > b.y;
        }

        if (a.y < 0 && b.y < 0) {
            return a.y < b.y;
        }

        return a.y < b.y;
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
        return a.x > b.x;
    } else {
        return f < s;
    }
}

bool deg_eq(long long int l, long long int r) {
    return atan2(ptr[l].y - ptr[zi].y, ptr[l].x - ptr[zi].x) ==
           atan2(ptr[r].y - ptr[zi].y, ptr[r].x - ptr[zi].x);
}

bool
is_line(long long int x1, long long int y1, long long int x2, long long int y2, long long int x3, long long int y3) {
    return (x3 - x1) * (y2 - y1) == (y3 - y1) * (x2 - x1);
}


int main() {
    cin >> n;

    for (int i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        ptr.push_back({x, y});
        if (x < ptr[zi].x || x == ptr[zi].x && y < ptr[zi].y) {
            zi = i;
        }
    }

    Point tmp = ptr[zi];

    for (int i = 0; i < n; ++i) {
        if (i != zi) {
            ptr[i].x -= ptr[zi].x;
            ptr[i].y -= ptr[zi].y;
        }
    }

    ptr[zi] = {0, 0};

    for (int i = 0; i < ptr.size(); ++i) {
        if ((ptr[i].x == ptr[zi].x) && (ptr[i].y == ptr[zi].y)) {
            continue;
        }
        indexes.push_back(i);
    }

    sort(indexes.begin(), indexes.end(), cmp);


    vector<int> new_indexes;
    for (int i = 0; i < indexes.size(); ++i) {
        new_indexes.push_back(indexes[i]);
        while (i < indexes.size() && ptr[indexes[i]].x == ptr[indexes[i + 1]].x &&
               ptr[indexes[i]].y == ptr[indexes[i + 1]].y) {
            i++;
        }
    }
    indexes = new_indexes;

    stack.push_back(zi);
    if (!indexes.empty()) {
        stack.push_back(indexes[0]);
    }

    for (int i = 1; i < indexes.size(); ++i) {
        while (l_turn(ptr[stack[stack.size() - 2]], ptr[stack[stack.size() - 1]], ptr[indexes[i]]) == -1) {
            stack.pop_back();
        }
        stack.push_back(indexes[i]);
    }

    vector<int> ans;
    ans.push_back(stack[0]);
    ans.push_back(stack[1]);

    for (int i = 2; i < stack.size(); ++i) {
        if (l_turn(ptr[ans[ans.size() - 2]], ptr[ans[ans.size() - 1]], ptr[stack[i]]) == 0) {
            if (dist(ptr[ans[ans.size() - 2]], ptr[ans[ans.size() - 1]]) <
                dist(ptr[ans[ans.size() - 2]], ptr[stack[i]])) {
                ans.pop_back();
            } else {
                continue;
            }
        }
        ans.push_back(stack[i]);
    }

    if (ans.size() > 2 && l_turn(ptr[ans[ans.size() - 2]], ptr[ans[ans.size() - 1]], ptr[ans[0]]) == 0) {
        ans.pop_back();
    }

    cout << ans.size() << "\n";
    for (int i :ans) {
        cout << ptr[i].x + tmp.x << " " << ptr[i].y + tmp.y << "\n";
    }

    return 0;
}
