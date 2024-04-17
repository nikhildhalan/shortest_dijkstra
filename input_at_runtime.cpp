#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int MAX_NODES = 1000; 
const int INF = INT_MAX;

vector<pii> graph[MAX_NODES];
int parent_forward[MAX_NODES];
int parent_backward[MAX_NODES];
int dist_forward[MAX_NODES];
int dist_backward[MAX_NODES];

pair<int, string> bidirectional_dijkstra(const vector<pii> graph[], int nodes, int source, int target) {
    priority_queue<pii, vector<pii>, greater<pii>> pq_forward, pq_backward;
    fill(dist_forward, dist_forward + nodes, INF);
    fill(dist_backward, dist_backward + nodes, INF);

    pq_forward.push({0, source});
    pq_backward.push({0, target});
    dist_forward[source] = 0;
    dist_backward[target] = 0;

    int meeting_point = -1;
    int min_dist = INF;

    while (!pq_forward.empty() && !pq_backward.empty()) {
        auto [dist_f, u_f] = pq_forward.top(); pq_forward.pop();
        auto [dist_b, u_b] = pq_backward.top(); pq_backward.pop();

        for (auto& [v, weight] : graph[u_f]) {
            int new_dist = dist_forward[u_f] + weight;
            if (new_dist < dist_forward[v]) {
                dist_forward[v] = new_dist;
                parent_forward[v] = u_f;
                pq_forward.push({new_dist, v});
            }
        }

        for (auto& [v, weight] : graph[u_b]) {
            int new_dist = dist_backward[u_b] + weight;
            if (new_dist < dist_backward[v]) {
                dist_backward[v] = new_dist;
                parent_backward[v] = u_b;
                pq_backward.push({new_dist, v});
            }
        }

        if (dist_forward[u_b] != INF && dist_backward[u_b] != INF) {
            int total_dist = dist_forward[u_b] + dist_backward[u_b];
            if (total_dist < min_dist) {
                min_dist = total_dist;
                meeting_point = u_b;
            }
        }
    }

    if (meeting_point == -1) {
        return {INF, ""};
    }

    string path_forward = to_string(source);
    for (int at = source; at != meeting_point; at = parent_forward[at]) {
        path_forward += " -> " + to_string(parent_forward[at]);
    }

    string path_backward = to_string(target);
    for (int at = target; at != meeting_point; at = parent_backward[at]) {
        path_backward = to_string(parent_backward[at]) + " -> " + path_backward;
    }

    return {min_dist, path_forward + " -> " + path_backward};
}

int main() {
    int nodes, edges;
    cout << "Enter the number of nodes and edges: ";
    cin >> nodes >> edges;

    cout << "Enter edges in the format (source destination weight):" << endl;
    for (int i = 0; i < edges; ++i) {
        int source, destination, weight;
        cin >> source >> destination >> weight;
        graph[source].push_back({destination, weight});
    }

    int source, target;
    cout << "Enter source and target nodes: ";
    cin >> source >> target;

    auto start = chrono::high_resolution_clock::now();
    auto [distance, path] = bidirectional_dijkstra(graph, nodes, source, target);
    auto end = chrono::high_resolution_clock::now();

    cout << "Shortest path length: " << distance << endl;
    cout << "Path: " << path << endl;

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Execution time: " << duration << " milliseconds" << endl;

    return 0;
}