#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

struct Point {
    long long x;
    long long y;
};

int n;
long long h = LONG_MAX;

Point *ptr;

Point f, s;

bool cmp_x(const Point &a, const Point &b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool cmp_y(const Point &a, const Point &b) {
    return a.y < b.y;
}

long long dst(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void get_pair(int l, int r) {
    if (r - l + 1 <= 100) {
        for (int i = l; i <= r; ++i) {
            for (int j = i + 1; j <= r; ++j) {
                long long d = dst(ptr[i], ptr[j]);
                if (d < h) {
                    h = d;
                    f = ptr[i];
                    s = ptr[j];
                }
            }
        }
        sort(ptr + l, ptr + r + 1, &cmp_y);
        return;
    }

    int mid = (l + r) / 2;
    get_pair(l, mid);
    get_pair(mid + 1, r);

    long long mid_x = ptr[mid].x;

    auto tmp = new Point[r - l + 1];

    merge(ptr + l, ptr + mid + 1, ptr + mid + 1, ptr + r + 1, tmp, &cmp_y);

    copy(tmp, tmp + (r - l + 1), ptr + l);

    for (int i = r; i >= l; --i) {
        if ((mid_x - ptr[i].x)*(mid_x - ptr[i].x) < h) {
            int j = i - 1;
            for (; j >= l && (ptr[i].y - ptr[j].y)*(ptr[i].y - ptr[j].y) < h; --j) {
                long long d = dst(ptr[i], ptr[j]);
                if (d < h) {
                    h = d;
                    f = ptr[i];
                    s = ptr[j];
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    cin >> n;

    ptr = new Point[n];
    long long x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        ptr[i] = {x, y};
    }

    sort(ptr, ptr + n, &cmp_x);

    get_pair(0, n - 1);

    cout << f.x << " " << f.y << "\n" << s.x << " " << s.y;

    return 0;
}
