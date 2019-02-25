#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <climits>

using namespace std;

struct Rect {
    double x_1 = INT_MIN;
    double y_1 = INT_MIN;
    double x_2 = INT_MAX;
    double y_2 = INT_MAX;
    Rect *r1 = nullptr;
    Rect *r2 = nullptr;
    Rect *r3 = nullptr;
    Rect *r4 = nullptr;
    vector<pair<double, double>> points;
    bool end = false;

    Rect(double f, double s, double t, double r, vector<pair<double, double>> &p) {
        x_1 = f;
        y_1 = s;
        x_2 = t;
        y_2 = r;

        if (p.size() > 4) {
            double cx = (x_1 + x_2) / 2;
            double cy = (y_1 + y_2) / 2;

            vector<pair<double, double>> p1;
            vector<pair<double, double>> p2;
            vector<pair<double, double>> p3;
            vector<pair<double, double>> p4;

            for (auto &i : p) {
                if (i.first > cx) {
                    if (i.second > cy) {
                        p2.push_back(i);
                    } else {
                        p4.push_back(i);
                    }
                } else {
                    if (i.second > cy) {
                        p1.push_back(i);
                    } else {
                        p3.push_back(i);
                    }
                }
            }

            if (!p1.empty()) r1 = new Rect(cx, y_1, x_2, cy, p1);
            if (!p2.empty()) r2 = new Rect(x_1, y_1, cx, cy, p2);
            if (!p3.empty()) r3 = new Rect(cx, cy, x_2, y_2, p3);
            if (!p4.empty()) r4 = new Rect(x_1, cy, cx, y_2, p4);
        } else {
            points = p;
            end = true;
        }
    }
};

int n, m;
map<pair<int, int>, int> id;

Rect *top;

double distance(pair<double, double> &pair, double x, double y) {
    double xx = x - pair.first;
    double yy = y - pair.second;
    return sqrt(xx * xx + yy * yy);
}

double dist_to_rect(Rect *r, pair<double, double> &pair) {
    double wx = (r->x_1 - r->x_2) / 2;
    double wy = (r->y_1 - r->y_2) / 2;
    double cx = (r->x_1 + r->x_2) / 2;
    double cy = (r->y_1 + r->y_2) / 2;

    if (pair.first < cx + wx &&
        pair.first > cx - wx) {
        return min(abs(pair.second - (cy + wy)), abs(pair.second - (cy - wy)));
    }

    if (pair.second < cy + wy &&
        pair.second > cy - wy) {
        return min(abs(pair.first - (cx + wx)),
                   abs(pair.first - (cx - wx)));
    }

    double d5 = distance(pair, cx + wx, cy - wy);
    double d6 = distance(pair, cx + wx, cy + wy);
    double d7 = distance(pair, cx - wx, cy - wy);
    double d8 = distance(pair, cx - wx, cy + wy);

    return min(d5, min(d6, min(d7, d8)));
}

void update(Rect *r, pair<double, double> *pair, double x, double y) {
    if (r == nullptr) {
        return;
    }

    if (r->end) {
        for (auto &point : r->points) {
            if (distance(point, x, y) < distance(*pair, x, y)) {
                pair->first = point.first;
                pair->second = point.second;
            }
        }
        return;
    }

    std::pair<double, double> cur_point = {x, y};

    if (distance(*pair, x, y) > dist_to_rect(r, cur_point)) {
        update(r->r1, pair, x, y);
        update(r->r2, pair, x, y);
        update(r->r3, pair, x, y);
        update(r->r4, pair, x, y);
    }
}

pair<double, double> closest_point(double x, double y, Rect *r) {
    if (r == nullptr) {
        return {20000, 20000}; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    if (r->end) {
        pair<double, double> pp = r->points[0];
        for (int i = 1; i < r->points.size(); ++i) {
            if (distance(r->points[i], x, y) < distance(pp, x, y)) {
                pp = r->points[i];
            }
        }
        return pp;
    }

    pair<double, double> pt;
    int num;

    double cx = (r->x_1 + r->x_2) / 2;
    double cy = (r->y_1 + r->y_2) / 2;

    if (x > cx) {
        if (y > cy) {
            pt = closest_point(x, y, r->r2);
            num = 2;
        } else {
            num = 4;
            pt = closest_point(x, y, r->r4);
        }
    } else {
        if (y > cy) {
            num = 1;
            pt = closest_point(x, y, r->r1);
        } else {
            num = 3;
            pt = closest_point(x, y, r->r3);
        }
    }

    if (num != 1) {
        update(r->r1, &pt, x, y);
    }

    if (num != 2) {
        update(r->r2, &pt, x, y);
    }

    if (num != 3) {
        update(r->r3, &pt, x, y);
    }

    if (num != 4) {
        update(r->r4, &pt, x, y);
    }

    return pt;
}

int main() {
    cin >> n >> m;

    double x_11 = INT_MIN;
    double y_11 = INT_MIN;
    double x_22 = INT_MAX;
    double y_22 = INT_MAX;

    vector<pair<double, double>> ptr;

    int x, y; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        id[{x, y}] = i + 1;
        ptr.push_back({x, y});
        if (x > x_11) {
            x_11 = x;
        }
        if (x < x_22) {
            x_22 = x;
        }
        if (y > y_11) {
            y_11 = y;
        }
        if (y < y_22) {
            y_22 = y;
        }
    }

    vector<pair<int, int>> q;

    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        q.push_back({x, y});
        if (x > x_11) {
            x_11 = x;
        }
        if (x < x_22) {
            x_22 = x;
        }
        if (y > y_11) {
            y_11 = y;
        }
        if (y < y_22) {
            y_22 = y;
        }
    }

    top = new Rect(x_11, y_11, x_22, y_22, ptr);

    for (int i = 0; i < m; ++i) {
        pair<double, double> ans = closest_point(q[i].first, q[i].second, top);

        cout << id[{(int) ans.first, (int) ans.second}] << "\n";
    }

    return 0;
}
