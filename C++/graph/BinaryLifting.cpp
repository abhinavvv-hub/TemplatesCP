class BinaryLifting {
    int n;
    int max_log;
    vector<int> depth;
    vector<vector<int>> up;
    void dfs(int u, int p, int d, const vector<vector<int>>& adj) {
        depth[u] = d;
        up[u][0] = p;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1, adj);
            }
        }
    }

public:
    BinaryLifting(int n, int root, const vector<vector<int>>& adj) : n(n) {
        max_log = log2(n) + 1;
        depth.assign(n, 0);
        up.assign(n, vector<int>(max_log, root));
        dfs(root, root, 0, adj);
        for (int j = 1; j < max_log; j++) {
            for (int i = 0; i < n; i++) {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }
    int getKthAncestor(int u, int k) {
        if (k > depth[u]) return -1;
        for (int j = 0; j < max_log; j++) {
            if ((k >> j) & 1) {
                u = up[u][j];
            }
        }
        return u;
    }
    int getLCA(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = getKthAncestor(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int j = max_log - 1; j >= 0; j--) {
            if (up[u][j] != up[v][j]) {
                u = up[u][j];
                v = up[v][j];
            }
        }
        return up[u][0];
    }
    int getDistance(int u, int v) {
        int lca = getLCA(u, v);
        return depth[u] + depth[v] - 2 * depth[lca];
    }
};
