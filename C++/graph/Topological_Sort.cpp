vector<int> topoSort() {
    vector<int> indeg(n+1,0);
    for (int u = 1; u <= n; u++) {
        for (auto [v, w]: adj[u]) {
            indeg[v]++;
        }
    }
    queue<int> q;
    for(int i = 1; i <= n; i++) {
            if(indeg[i] == 0) {
                q.push(i);
            }
    }
    vector<int> topo;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for(auto [v, w]: adj[u]) {
            indeg[v]--;
            if(indeg[v] == 0) {
                q.push(v);
            }
        }
    }
    return topo;
}
