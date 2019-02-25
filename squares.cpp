#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <unordered_map>
#include <iomanip>

using namespace std;

struct Line {
    long double x1;
    long double y1;
    long double x2;
    long double y2;
};

vector<Line> lines;
map<pair<long double, long double>, int> vertexes;
vector<vector<int>> G(10000);
vector<vector<bool>> b(10000);
vector<pair<long double, long double>> V;
int index = 0, sort_index = 0;
vector<long double> answers(0);

bool check_parallel(Line f, Line s) {
    if ((f.x1 - f.x2) * (s.y1 - s.y2) - (s.x1 - s.x2) * (f.y1 - f.y2) == 0) {
        return false;
    } else {
        return true;
    }
}

pair<long double, long double> get_point(Line f, Line s) {
    pair<long double, long double> p;
    p.first = (((f.x1 * f.y2 - f.y1 * f.x2) * (s.x1 - s.x2) - (f.x1 - f.x2) * (s.x1 * s.y2 - s.y1 * s.x2)) /
               ((f.x1 - f.x2) * (s.y1 - s.y2) - (s.x1 - s.x2) * (f.y1 - f.y2)));
    p.second = (((f.x1 * f.y2 - f.y1 * f.x2) * (s.y1 - s.y2) - (f.y1 - f.y2) * (s.x1 * s.y2 - s.y1 * s.x2)) /
                ((f.x1 - f.x2) * (s.y1 - s.y2) - (s.x1 - s.x2) * (f.y1 - f.y2)));
    return p;
};

bool polar_deg(int l, int r) {
    return atan2(V[l].second - V[sort_index].second, V[l].first - V[sort_index].first) <
           atan2(V[r].second - V[sort_index].second, V[r].first - V[sort_index].first);
}

int main() {
    int n;
    cin >> n;
    if (n < 3) {
        cout << 0;
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        Line l;
        cin >> l.x1 >> l.y1 >> l.x2 >> l.y2;
        lines.push_back(l);
    }
    for (int i = 0; i < n; ++i) {
        vector<pair<long double, long double>> sorted_points, points;
        for (int j = 0; j < n; ++j) {
            if (check_parallel(lines[i], lines[j])) {
                pair<long double, long double> poin = get_point(lines[i], lines[j]);
                bool tr = true;
                for (int k = 0; k < points.size(); ++k) {
                    if (points[k] == poin) {
                        tr = false;
                        break;
                    }
                }
                if (tr) {
                    points.push_back(poin);
                }
            }
        }

        if (lines[i].x1 == lines[i].x2) {
            for (int j = 0; j < points.size(); ++j) {
                sorted_points.push_back({points[j].second, j});
            }
        } else {
            for (int j = 0; j < points.size(); ++j) {
                sorted_points.push_back({points[j].first, j});
            }
        }

        sort(sorted_points.begin(), sorted_points.end());
        if (sorted_points.size() < 2) {
            continue;
        }
        for (int j = 0; j < sorted_points.size() - 1; ++j) {
            pair<long double, long double> u = points[sorted_points[j].second];
            pair<long double, long double> v = points[sorted_points[j + 1].second];
            auto u_iter = vertexes.find(u);
            auto v_iter = vertexes.find(v);
            int u_index;
            int v_index;

            if (u_iter != vertexes.end()) {
                u_index = vertexes[u];
            } else {
                u_index = index;
                V.push_back(u);
                vertexes.insert({u, index++});
            }

            if (v_iter != vertexes.end()) {
                v_index = vertexes[v];
            } else {
                v_index = index;
                V.push_back(v);
                vertexes.insert({v, index++});
            }
            G[u_index].push_back(v_index);
            b[u_index].push_back(false);
            G[v_index].push_back(u_index);
            b[v_index].push_back(false);
        }
    }

    for (int i = 0; i < G.size(); ++i) {
        stable_sort(G[i].begin(), G[i].end(), polar_deg);
        sort_index++;
    }
    for (int i = 0; i < index; ++i) {
        for (int j = 0; j < G[i].size(); ++j)
            if (!b[i][j]) {
                b[i][j] = true;
                int v = G[i][j];
                int cur = i;
                vector<int> gr;
                for (;;) {
                    gr.push_back(v);
                    int ind = 0;
                    for (int k = 0; k < G[v].size(); ++k) {
                        if (G[v][k] == cur) {
                            ind = k;
                            break;
                        }
                    }
                    ind++;
                    if (ind == G[v].size()) {
                        ind = 0;
                    }
                    if (b[v][ind]) {
                        break;
                    }
                    b[v][ind] = true;
                    cur = v;
                    v = G[v][ind];
                }

                long double S = 0;
                for (int k = 0; k < gr.size() - 1; ++k) {
                    S += V[gr[k]].first * V[gr[k + 1]].second;
                }
                S += V[gr[gr.size() - 1]].first * V[gr[0]].second;
                for (int k = 0; k < gr.size() - 1; ++k) {
                    S -= V[gr[k + 1]].first * V[gr[k]].second;
                }
                S -= V[gr[0]].first * V[gr[gr.size() - 1]].second;
                S = abs(S) / 2;
                if (S >= 0.00000001) {
                    answers.push_back(S);
                }
            }
    }
    sort(answers.begin(), answers.end());
    if (answers.size() < 1) {
        cout << 0;
        return 0;
    }

    cout << answers.size() - 1 << endl;
    for (int i = 0; i < answers.size() - 1; ++i) {
        cout << setprecision(15) << answers[i] << endl;
    }
    return 0;
}

/*
 6
 0 0 1 1
 1 0 2 8
 0 6 1 6
 0 4 1 5
 0 9 1 8
 2 1 4 7
 */
