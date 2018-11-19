#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct Pryanik {
    short x = 0;
    short y = 0;
    short width = -1;

    Pryanik(short xx, short yy, short ww) {
        x = xx;
        y = yy;
        width = ww;
    }

    Pryanik() {
        x = 0;
        y = 0;
        width = -1;
    }

    bool have_point = false;
    bool have_one_point = false;

    pair<short, short> point;
};

vector<Pryanik> vertexes;
map<pair<short, short>, int> id;


void build(short x, short y, short width, int index) {
    vertexes[index] = Pryanik(x, y, width);

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
    if (set_point(x, y, index)) {
        return;
    }

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

double distance(pair<short, short> *pair, short x, short y) {
    int xx = pair->first;
    int yy = pair->second;
    return sqrt((x - xx) * (x - xx) + (y - yy) * (y - yy));
}

double to_pryanik(int index, pair<short, short> *pair) {
    if (pair->first < vertexes[index].x + vertexes[index].width / 2 &&
        pair->first > vertexes[index].x - vertexes[index].width / 2) {
        return min(abs(pair->second - (vertexes[index].y + vertexes[index].width / 2)), abs(pair->second - (vertexes[index].y - vertexes[index].width / 2)));
    }

    if (pair->second < vertexes[index].y + vertexes[index].width / 2 &&
        pair->second > vertexes[index].y - vertexes[index].width / 2) {
        return min(abs(pair->first - (vertexes[index].x + vertexes[index].width / 2)), abs(pair->first - (vertexes[index].x - vertexes[index].width / 2)));
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
//    cout << index << "\n";
//    if(p == nullptr) {
//        return;
//    }
//    cout << "up: " << p->x << " " << p->y << "\n";
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

    std::pair<short, short> nw = {x, y};

    if (distance(pair, x, y) <= to_pryanik(index, &nw)) {
        return;
    } else {
//        cout << "from up" << endl;
//        cout << vertexes[index].have_point << " " << vertexes[index].have_one_point << " " << vertexes[index].width
//             << endl;
        update(index * 4 + 1, pair, x, y);
        update(index * 4 + 2, pair, x, y);
        update(index * 4 + 3, pair, x, y);
        update(index * 4 + 4, pair, x, y);
    }
}

pair<short, short> closest_point(short x, short y, int index) {
//    cout << "c_p: " << p->x << " " << p->y << "\n";
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
        if (j == num) {
            continue;
        }
        update(index * 4 + j + 1, &pt, x, y);
    }

    return pt;
}

int main() {
    vertexes.resize(5592405);
    build(0, 0, 4096, 0);

    int n, m;

    cin >> n >> m;

    for (int i = 0; i < n; ++i) {
        short x, y;
        cin >> x >> y;
        id[{x, y}] = i + 1;
        add(x * 2 + 1, y * 2 + 1, 0);
    }

    for (int i = 0; i < m; ++i) {
        short x, y;
        cin >> x >> y;
        x = x * 2 + 1;
        y = y * 2 + 1;

        pair<short, short> ans = closest_point(x, y, 0);

//        cout << ans.first << " " << ans.second << "\n";

        cout << id[{(ans.first - 1) / 2, (ans.second - 1) / 2}] << "\n";
    }

    return 0;
}

/*
6 1
0 3
3 3
-3 2
-4 -1
-2 -2
-2 -4
0 0

4 1
1001 1001
1001 -1000
-1001 1000
-1001 -1000
0 0

2 2
10 2
3 3
-4 -9
-6 -1
*/