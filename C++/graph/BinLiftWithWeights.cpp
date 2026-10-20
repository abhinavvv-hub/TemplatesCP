class BinaryLifting {
    int n;
    int max_log;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<long long>> sum_weight; 
    void dfs(int u, int p, int d, long long w_to_p, const vector<vector<pair<int, int>>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        sum_weight[u][0] = w_to_p;
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (v != p) {
                dfs(v, u, d + 1, w, adj);
            }
        }
    }

public:
    BinaryLifting(int n, int root, const vector<vector<pii>>& adj) : n(n) {
        max_log = log2(n) + 1;
        depth.assign(n, 0);
        up.assign(n, vector<int>(max_log, root));
        sum_weight.assign(n, vector<long long>(max_log, 0));
        dfs(root, root, 0, 0, adj);
        for (int j = 1; j < max_log; j++) {
            for (int i = 0; i < n; i++) {
                int ancestor = up[i][j - 1];
                up[i][j] = up[ancestor][j - 1];
                sum_weight[i][j] = sum_weight[i][j - 1] + sum_weight[ancestor][j - 1];
            }
        }
    }
    pair<int, long long> getKthAncestorWithWeight(int u, int k) {
        if (k > depth[u]) return {-1, 0};
        long long weights_collected = 0;
        for (int j = 0; j < max_log; j++) {
            if ((k >> j) & 1) {
                weights_collected += sum_weight[u][j];
                u = up[u][j];
            }
        }
        return {u, weights_collected};
    }

    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = getKthAncestorWithWeight(u, depth[u] - depth[v]).first;
        if (u == v) return u;
        for (int j = max_log - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }
    long long getPathWeightSum(int u, int v) {
        long long total_sum = 0;
        if (depth[u] < depth[v]) swap(u, v);
        auto lift = getKthAncestorWithWeight(u, depth[u] - depth[v]);
        u = lift.first;
        total_sum += lift.second;
        if (u == v) return total_sum;
        for (int j = max_log - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                total_sum += sum_weight[u][j] + sum_weight[v][j];
                u = up[u][j];
                v = up[v][j];
            }
        }
        total_sum += sum_weight[u][0] + sum_weight[v][0];
        return total_sum;
    }
    int getNodeOnPath(int u, int v, int k) {
        int lca = getLCA(u, v);
        int left_len = depth[u] - depth[lca] + 1;
        int total_nodes = depth[u] + depth[v] - 2 * depth[lca] + 1;
        if (k < 1 || k > total_nodes) return -1; 
        if (k <= left_len) {
            return getKthAncestorWithWeight(u, k - 1).first; 
        }
        else {
            int steps_from_v = total_nodes - k;
            return getKthAncestorWithWeight(v, steps_from_v).first;
        }
    }
};
