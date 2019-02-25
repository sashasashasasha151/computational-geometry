#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iomanip>

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

long long dim(vector<int> ans) {
    int l = 0;
    int r = 0;
    int l_n, r_n;

    for (int i = 0; i < ans.size(); ++i) {
        if (ptr[ans[i]].x > ptr[ans[r]].x || (ptr[ans[i]].x == ptr[ans[r]].x && ptr[ans[i]].y > ptr[ans[r]].y)) {
            r = i;
        }
    }

    l_n = (l + 1) % ans.size();
    r_n = (r + 1) % ans.size();

    long long ret = dist(ptr[ans[l]], ptr[ans[r]]);

    int trn = l_turn(ptr[ans[l_n]], ptr[ans[l]], {ptr[ans[r_n]].x - (ptr[ans[r]].x - ptr[ans[l]].x),
                                                  ptr[ans[r_n]].y - (ptr[ans[r]].y - ptr[ans[l]].y)});

    if (trn == -1) {
        swap(l, r);
        swap(l_n, r_n);
    }


    int ll = l;
    int rr = r;

    l = l_n;
    l_n = (l + 1) % ans.size();

    while (!(l == ll && r == rr || l == rr && r == ll)) {
//        cout << l << " " << r << "\n";
        ret = max(ret, dist(ptr[ans[l]], ptr[ans[r]]));

        trn = l_turn(ptr[ans[l_n]], ptr[ans[l]], {ptr[ans[r_n]].x - (ptr[ans[r]].x - ptr[ans[l]].x), ptr[ans[r_n]].y - (ptr[ans[r]].y - ptr[ans[l]].y)});

        if (trn == -1) {
            swap(l, r);
            swap(l_n, r_n);
        }

//        cout << "hh\n";
        l = l_n;
        l_n = (l + 1) % ans.size();
    }

    return ret;
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

    if (ans.size() == 1) {
        cout << 0;
        return 0;
    }

    if (ans.size() == 2) {
        cout << setprecision(30) << sqrt((double) dist(ptr[ans[0]], ptr[ans[1]]));
        return 0;
    }

    cout << setprecision(30) << sqrt((double) dim(ans));

    return 0;
}
