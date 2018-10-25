#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

int n;
int sort_index = 0;
vector<pair<int, int>> V;
vector<int> indexes;
vector<int> stack;

int square(int x1, int y1, int x2, int y2, int x3, int y3) {
    return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

bool is_right(int x1, int y1, int x2, int y2, int x3, int y3) {
    return square(x1, y1, x2, y2, x3, y3) < 0;
}

bool polar_deg(int l, int r) {
    if (atan2(V[l].second - V[sort_index].second, V[l].first - V[sort_index].first) ==
        atan2(V[r].second - V[sort_index].second, V[r].first - V[sort_index].first)) {
        if (((V[l].first - V[sort_index].first) * (V[l].first - V[sort_index].first) +
             (V[l].second - V[sort_index].second) * (V[l].second - V[sort_index].second)) <
            ((V[r].first - V[sort_index].first) * (V[r].first - V[sort_index].first) +
             (V[r].second - V[sort_index].second) * (V[r].second - V[sort_index].second))) {
            return true;
        } else {}
        return false;
    }

    return atan2(V[l].second - V[sort_index].second, V[l].first - V[sort_index].first) <
           atan2(V[r].second - V[sort_index].second, V[r].first - V[sort_index].first);
}

bool deg_eq(int l, int r) {
    return atan2(V[l].second - V[sort_index].second, V[l].first - V[sort_index].first) ==
           atan2(V[r].second - V[sort_index].second, V[r].first - V[sort_index].first);
}

bool is_line(int x1, int y1, int x2, int y2, int x3, int y3) {
    return (x3 - x1) * (y2 - y1) == (y3 - y1) * (x2 - x1);
}


int main() {
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        V.emplace_back(x, y);
        if (y < V[sort_index].second) {
            sort_index = i;
        } else {
            if (y == V[sort_index].second && x < V[sort_index].first) {
                sort_index = i;
            }
        }
    }

    for (int i = 0; i < V.size(); ++i) {
        if ((V[i].first == V[sort_index].first) && (V[i].second == V[sort_index].second)) {
            continue;
        }
        indexes.push_back(i);
    }

    stable_sort(indexes.begin(), indexes.end(), polar_deg);

    vector<int> new_indexes;
    for (int i = 0; i < indexes.size(); ++i) {
        while (i < indexes.size()-1 && deg_eq(indexes[i], indexes[i + 1])) {
            i++;
        }
        new_indexes.push_back(indexes[i]);
    }
    indexes = new_indexes;

    stack.push_back(sort_index);
    if (!indexes.empty()) {
        stack.push_back(indexes[0]);
    }
    for (int i = 1; i < indexes.size(); ++i) {
        while (is_right(V[stack[stack.size() - 2]].first, V[stack[stack.size() - 2]].second,
                        V[stack[stack.size() - 1]].first, V[stack[stack.size() - 1]].second,
                        V[indexes[i]].first, V[indexes[i]].second)) {
            stack.pop_back();
        }
        stack.push_back(indexes[i]);
    }

    if (stack.size() == 2 || stack.size() == 1) {
        cout << stack.size() << "\n";
        for (int i :stack) {
            cout << V[i].first << " " << V[i].second << "\n";
        }
        return 0;
    }

    vector<int> ans;
    ans.push_back(stack[0]);
    ans.push_back(stack[1]);
    for (int i = 2; i < stack.size(); ++i) {
        if (is_line(V[ans[ans.size() - 2]].first, V[ans[ans.size() - 2]].second,
                    V[ans[ans.size() - 1]].first, V[ans[ans.size() - 1]].second,
                    V[stack[i]].first, V[stack[i]].second)) {
            ans.pop_back();
            ans.push_back(stack[i]);
            continue;
        }
        ans.push_back(stack[i]);
    }

    if (is_line(V[ans[ans.size() - 2]].first, V[ans[ans.size() - 2]].second,
                V[ans[ans.size() - 1]].first, V[ans[ans.size() - 1]].second,
                V[ans[0]].first, V[ans[0]].second)) {
        ans.pop_back();
    }


    cout << ans.size() << "\n";
    for (int i :ans) {
        cout << V[i].first << " " << V[i].second << "\n";
    }

    return 0;
}
/*
6
0 2
1 0
2 0
3 0
4 2
3 10

3
0 0
1 0
2 0

3
2 0
0 0
1 0

7
0 0
1 0
4 3
2 1
3 2
0 1
2 2

10
0 0
4 2
2 1
2 2
1 1
3 6
1 2
2 4
0 3
0 5
 */