#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct Rect {
    short x{};
    short y{};
    short width{};
    bool have_point = false;
    bool have_one_point = false;

    pair<short, short> point;

    Rect() = default;

    Rect(short xx, short yy, short ww) : x(xx), y(yy), width(ww) {}
};

vector<Rect> vertexes;
map<pair<short, short>, int> id;


void build(short x, short y, short width, int index) {
    vertexes[index] = Rect(x, y, width);

    if (width == 2) {
        return;
    }

    build(x - width / 4, y + width / 4, width / 2, index * 4 + 1);
    build(x - width / 4, y - width / 4, width / 2, index * 4 + 2);
    build(x + width / 4, y + width / 4, width / 2, index * 4 + 3);
    build(x + width / 4, y - width / 4, width / 2, index * 4 + 4);
}

bool set_point(short x, short y, int index) {
    if (vertexes[index].x == x && vertexes[index].y == y) {
        vertexes[index].have_one_point = true;
        vertexes[index].have_point = true;
        vertexes[index].point = {x, y};
        return true;
    }

    if (!vertexes[index].have_point) {
        vertexes[index].have_one_point = true;
        vertexes[index].have_point = true;
        vertexes[index].point = {x, y};
        return false;
    }

    if (vertexes[index].have_one_point) {
        vertexes[index].have_one_point = false;
    }

    return false;
}

void add(short x, short y, int index) {
    if (!set_point(x, y, index)) {
        if (x > vertexes[index].x) {
            if (y > vertexes[index].y) {
                add(x, y, index * 4 + 3);
            } else {
                add(x, y, index * 4 + 4);
            }
        } else {
            if (y > vertexes[index].y) {
                add(x, y, index * 4 + 1);
            } else {
                add(x, y, index * 4 + 2);
            }
        }
    }
}

double distance(pair<short, short> *pair, short x, short y) {
    int xx = x - pair->first;
    int yy = y - pair->second;
    return sqrt(xx * xx + yy * yy);
}

double dist_to_rect(int index, pair<short, short> *pair) {
    if (pair->first < vertexes[index].x + vertexes[index].width / 2 &&
        pair->first > vertexes[index].x - vertexes[index].width / 2) {
        return min(abs(pair->second - (vertexes[index].y + vertexes[index].width / 2)),
                   abs(pair->second - (vertexes[index].y - vertexes[index].width / 2)));
    }

    if (pair->second < vertexes[index].y + vertexes[index].width / 2 &&
        pair->second > vertexes[index].y - vertexes[index].width / 2) {
        return min(abs(pair->first - (vertexes[index].x + vertexes[index].width / 2)),
                   abs(pair->first - (vertexes[index].x - vertexes[index].width / 2)));
    }

    double d5 = distance(pair, vertexes[index].x + vertexes[index].width / 2,
                         vertexes[index].y - vertexes[index].width / 2);
    double d6 = distance(pair, vertexes[index].x + vertexes[index].width / 2,
                         vertexes[index].y + vertexes[index].width / 2);
    double d7 = distance(pair, vertexes[index].x - vertexes[index].width / 2,
                         vertexes[index].y - vertexes[index].width / 2);
    double d8 = distance(pair, vertexes[index].x - vertexes[index].width / 2,
                         vertexes[index].y + vertexes[index].width / 2);
    return min(d5, min(d6, min(d7, d8)));
}

void update(int index, pair<short, short> *pair, short x, short y) {
    if (!vertexes[index].have_point) {
        return;
    }

    if (vertexes[index].have_one_point) {
        if (distance(&vertexes[index].point, x, y) < distance(pair, x, y)) {
            pair->first = vertexes[index].point.first;
            pair->second = vertexes[index].point.second;
        }
        return;
    }

    std::pair<short, short> cur_point = {x, y};

    if (distance(pair, x, y) > dist_to_rect(index, &cur_point)) {
        for (int i = 1; i <= 4; ++i) {
            update(index * 4 + i, pair, x, y);
        }
    }
}

pair<short, short> closest_point(short x, short y, int index) {
    if (vertexes[index].have_one_point) {
        return vertexes[index].point;
    }

    if (!vertexes[index].have_point) {
        return {20000, 20000};
    }

    pair<short, short> pt;
    int num;

    if (x > vertexes[index].x) {
        if (y > vertexes[index].y) {
            pt = closest_point(x, y, index * 4 + 3);
            num = 2;
        } else {
            num = 3;
            pt = closest_point(x, y, index * 4 + 4);
        }
    } else {
        if (y > vertexes[index].y) {
            num = 0;
            pt = closest_point(x, y, index * 4 + 1);
        } else {
            num = 1;
            pt = closest_point(x, y, index * 4 + 2);
        }
    }

    for (int j = 0; j < 4; ++j) {
        if (j != num) {
            update(index * 4 + j + 1, &pt, x, y);
        }
    }

    return pt;
}

int main() {
    vertexes.resize(5592405);
    build(0, 0, 4096, 0);

    int n, m;
    cin >> n >> m;

    short x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        id[{x, y}] = i + 1;
        add(x * 2 + 1, y * 2 + 1, 0);
    }

    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        x = x * 2 + 1;
        y = y * 2 + 1;

        pair<short, short> ans = closest_point(x, y, 0);

        cout << id[{(ans.first - 1) / 2, (ans.second - 1) / 2}] << "\n";
    }

    return 0;
}