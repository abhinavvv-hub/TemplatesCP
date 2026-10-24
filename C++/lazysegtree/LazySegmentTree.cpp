template <typename T>
class LazySegmentTree {
    vector<T> Segment, Lazy;
    int sz;

    int leftNode(int n) { return 2 * n + 1; }
    int rightNode(int n) { return 2 * n + 2; }
    int mid(int l, int r) { return l + (r - l) / 2; }

    void push(int node, int start, int end) {
        if (Lazy[node] == 0) return;

        if (start != end) {
            Segment[leftNode(node)] += Lazy[node];
            Lazy[leftNode(node)] += Lazy[node];
            
            Segment[rightNode(node)] += Lazy[node];
            Lazy[rightNode(node)] += Lazy[node];
        }

        Lazy[node] = 0;
    }

    void build(const vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            Segment[node] = 0;
            return;
        }

        int m = mid(start, end);
        build(arr, leftNode(node), start, m);
        build(arr, rightNode(node), m + 1, end);

        Segment[node] = (Segment[leftNode(node)] + Segment[rightNode(node)]);
    }

    void updateRange_(int node, int start, int end, int l, int r, T val) {
        if (r < start || end < l) return;

        if (l <= start && end <= r) {
            Segment[node] += val;
            Lazy[node] += val;
            return;
        }

        push(node, start, end);
        int m = mid(start, end);

        updateRange_(leftNode(node), start, m, l, r, val);
        updateRange_(rightNode(node), m + 1, end, l, r, val);

        Segment[node] = (Segment[leftNode(node)] + Segment[rightNode(node)]);
    }

    T query_(int node, int start, int end, int l, int r) {
        if (r < start || end < l)
            return 0;

        if (l <= start && end <= r)
            return Segment[node];

        push(node, start, end);
        int m = mid(start, end);

        T left = query_(leftNode(node), start, m, l, r);
        T right = query_(rightNode(node), m + 1, end, l, r);

        return left + right;
    }

public:
    explicit LazySegmentTree(const vector<T>& arr): sz(static_cast<int>(arr.size())) {
        Segment.assign(4 * max(1, sz), 0);
        Lazy.assign(4 * max(1, sz), T{});

        if (sz > 0) build(arr, 0, 0, sz - 1);
    }

    void updateRange(int l, int r, T val) {
        if (sz == 0 || l > r || l < 0 || r >= sz) return;
        updateRange_(0, 0, sz - 1, l, r, val);
    }

    void update(int idx, T val) {
        updateRange(idx, idx, val);
    }

    T query(int l, int r) {
        if (sz == 0 || l > r || l < 0 || r >= sz) return 0;
        return query_(0, 0, sz - 1, l, r);
    }
};
