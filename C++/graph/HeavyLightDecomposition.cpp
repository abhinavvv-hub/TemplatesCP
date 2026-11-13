const int segSentinel = 0;
const int segLazySentinel = -1;
const int segInitVal = 0;

template<typename T>
class HeavyLightDecomposition {
    int n, h;
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
    vector<int> segLen;
    vector<T> baseArr;

    inline T segCombine(T a, T b) {
        return max(a, b);
    }

    inline T segLazyApply(T lazyVal, T newVal) {
        return newVal;
    }

    inline T segLazyFunc(T curVal, T lazyVal, int len) {
        return lazyVal;
    }

    void segApplyNode(int p, T val) {
        segTree[p] = segLazyFunc(segTree[p], val, segLen[p]);
        if (p < n) {
            segLazy[p] = (segLazy[p] == segLazySentinel) ? val : segLazyApply(segLazy[p], val);
        }
    }

    void segBuildUp(int p) {
        while (p > 1) {
            p >>= 1;
            segTree[p] = segCombine(segTree[p << 1], segTree[p << 1 | 1]);
            if (segLazy[p] != segLazySentinel) {
                segTree[p] = segLazyFunc(segTree[p], segLazy[p], segLen[p]);
            }
        }
    }

    void segPushDown(int p) {
        for (int s = h; s > 0; --s) {
            int i = p >> s;
            if (segLazy[i] != segLazySentinel) {
                segApplyNode(i << 1, segLazy[i]);
                segApplyNode(i << 1 | 1, segLazy[i]);
                segLazy[i] = segLazySentinel;
            }
        }
    }

    void segBuildAll() {
        for (int i = 0; i < n; i++) {
            segTree[n + i] = baseArr[i];
            segLen[n + i] = 1;
        }
        for (int i = n - 1; i > 0; --i) {
            segTree[i] = segCombine(segTree[i << 1], segTree[i << 1 | 1]);
            segLen[i] = segLen[i << 1] + segLen[i << 1 | 1];
        }
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
        label[v] = labelTime;
        baseArr[labelTime++] = arr[v];
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
        h = 32 - __builtin_clz(n);
        edges = adj;
        bigChild.assign(n, -1);
        sz.assign(n, 0);
        depth.assign(n, 0);
        chain.resize(n);
        for (int i = 0; i < n; i++) chain[i] = i;
        label.assign(n, 0);
        labelTime = 0;
        par.assign(n, -1);

        segTree.assign(2 * n, segInitVal);
        segLazy.assign(n, segLazySentinel);
        segLen.assign(2 * n, 0);
        baseArr.assign(n, segInitVal);

        dfsSize(root, -1, 0);
        dfsChains(root, -1);

        vector<T> initialArr = arr.empty() ? vector<T>(n, segInitVal) : arr;
        dfsLabels(initialArr, root, -1);
        segBuildAll();
    }

    T segQueryHeader(int l, int r) {
        l += n;
        r += n + 1;
        segPushDown(l);
        segPushDown(r - 1);
        T resL = segSentinel, resR = segSentinel;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resL = segCombine(resL, segTree[l++]);
            if (r & 1) resR = segCombine(segTree[--r], resR);
        }
        return segCombine(resL, resR);
    }

    void segUpdateHeader(int l, int r, T v) {
        l += n;
        r += n + 1;
        int l0 = l, r0 = r;
        segPushDown(l0);
        segPushDown(r0 - 1);
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) segApplyNode(l++, v);
            if (r & 1) segApplyNode(--r, v);
        }
        segBuildUp(l0);
        segBuildUp(r0 - 1);
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
