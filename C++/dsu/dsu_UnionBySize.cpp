class DSU {
    vector<int> parent;
    vector<int> size;
public:
    explicit DSU(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int findSet(int x) {
        if (parent[x] == x) return x;
        return parent[x] = findSet(parent[x]); 
    }
    void unionSet(int x, int y) {
        int px = findSet(x);
        int py = findSet(y);
        if (px == py) return;
        if (size[px] < size[py]) {
            swap(px, py);
        }
        parent[py] = px;
        size[px] += size[py];
    }
    int getSize(int x) {
        return size[findSet(x)];
    }
};
