const int segSentinel = 0;
const int segLazySentinel = -1;
const int segInitVal = 0;

template<typename T>
class HeavyLightDecomposition {
    int n;
    vector<vector<int>> edges;
    vector<int> bigChild;
    vector<int> sz;
    vector<int> depth;
    vector<int> chain;
    vector<int> label;
    int labelTime;
    vector<int> par;
    vector<T> segTree;
    vector<T> segLazy;

    inline T segCombine(T a, T b) {
        return a + b;
    }
    
    inline T segLazyApply(T lazyVal, T newVal) {
        return newVal;
    }
    
    inline T segLazyFunc(T curVal, T lazyVal, int l, int r) { 
        return lazyVal * (r - l + 1); 
    }

    T segQueryRec(int i, int tl, int tr, int ql, int qr) {
        segEvalLazy(i, tl, tr);
        if (ql <= tl && tr <= qr) return segTree[i];
        if (tl > tr || tr < ql || qr < tl) return segSentinel;
        int mid = (tl + tr) / 2;
        T a = segQueryRec(2 * i + 1, tl, mid, ql, qr);
        T b = segQueryRec(2 * i + 2, mid + 1, tr, ql, qr);
        return segCombine(a, b);
    }

    T segUpdateRec(int i, int tl, int tr, int ql, int qr, T v) {
        segEvalLazy(i, tl, tr);
        if (tl > tr || tr < ql || qr < tl) return segTree[i];
        if (ql <= tl && tr <= qr) {
            segLazy[i] = segLazyApply(segLazy[i], v);
            segEvalLazy(i, tl, tr);
            return segTree[i];
        }
        int mid = (tl + tr) / 2;
        T a = segUpdateRec(2 * i + 1, tl, mid, ql, qr, v);
        T b = segUpdateRec(2 * i + 2, mid + 1, tr, ql, qr, v);
        return segTree[i] = segCombine(a, b);
    }

    void segEvalLazy(int i, int l, int r) {
        if (segLazy[i] == segLazySentinel) return;
        segTree[i] = segLazyFunc(segTree[i], segLazy[i], l, r);
        if (l != r) {
            segLazy[i * 2 + 1] = segLazyApply(segLazy[i * 2 + 1], segLazy[i]);
            segLazy[i * 2 + 2] = segLazyApply(segLazy[i * 2 + 2], segLazy[i]);
        }
        segLazy[i] = segLazySentinel;
    }

    void dfsSize(int v, int p, int d) {
        sz[v] = 1;
        depth[v] = d;
        par[v] = p;
        int bigc = -1, bigv = -1;
        for (int x : edges[v]) {
            if (x != p) {
                dfsSize(x, v, d + 1);
                sz[v] += sz[x];
                if (sz[x] > bigv) {
                    bigc = x;
                    bigv = sz[x];
                }
            }
        }
        bigChild[v] = bigc;
    }

    void dfsChains(int v, int p) {
        if (bigChild[v] != -1) {
            chain[bigChild[v]] = chain[v];
            dfsChains(bigChild[v], v);
        }
        for (int x : edges[v]) {
            if (x != p && x != bigChild[v]) {
                dfsChains(x, v);
            }
        }
    }

    void dfsLabels(const vector<T>& arr, int v, int p) {
        label[v] = labelTime++;
        segUpdateHeader(label[v], label[v], arr[v]);
        if (bigChild[v] != -1) {
            dfsLabels(arr, bigChild[v], v);
        }
        for (int x : edges[v]) {
            if (x != p && x != bigChild[v]) {
                dfsLabels(arr, x, v);
            }
        }
    }

public:
    HeavyLightDecomposition(int nodesCount, int root, const vector<vector<int>>& adj, const vector<T>& arr = {}) {
        n = nodesCount;
        edges = adj;
        bigChild.assign(n, -1);
        sz.assign(n, 0);
        depth.assign(n, 0);
        chain.resize(n);
        for (int i = 0; i < n; i++) chain[i] = i;
        label.assign(n, 0);
        labelTime = 0;
        par.assign(n, -1);
        segTree.assign(4 * n, segInitVal);
        segLazy.assign(4 * n, segLazySentinel);
        dfsSize(root, -1, 0);
        dfsChains(root, -1);
        vector<T> initialArr = arr;
        if (initialArr.empty()) {
            initialArr.assign(n, segInitVal);
        }
        dfsLabels(initialArr, root, -1);
    }
    
    T segQueryHeader(int l, int r) { 
        return segQueryRec(0, 0, n - 1, l, r);
    }
    
    void segUpdateHeader(int l, int r, T v) {
        segUpdateRec(0, 0, n - 1, l, r, v); 
    }
    
    T query(int u, int v) {
        T val = segSentinel;
        while (chain[u] != chain[v]) {
            if (depth[chain[u]] < depth[chain[v]]) swap(u, v);
            val = segCombine(val, segQueryHeader(label[chain[u]], label[u]));
            u = par[chain[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        val = segCombine(val, segQueryHeader(label[v], label[u]));
        return val;
    }
    void update(int u, int v, T val) {
        while (chain[u] != chain[v]) {
            if (depth[chain[u]] < depth[chain[v]]) swap(u, v);
            segUpdateHeader(label[chain[u]], label[u], val);
            u = par[chain[u]];
        }
        if (depth[u] < depth[v]) swap(u, v);
        segUpdateHeader(label[v], label[u], val);
    }
};
