class LazySegmentTree {
    vector<long long> Segment;
    vector<long long> Lazy;
    int sz;

    int leftNode(int n) { return 2 * n + 1; }
    int rightNode(int n) { return 2 * n + 2; }
    int mid(int l, int r) { return l + (r - l) / 2; }
    void push(int node, int start, int end) {
        if (Lazy[node] != 0) {
            Segment[node] += Lazy[node];
            if (start != end) {
                Lazy[leftNode(node)] += Lazy[node];
                Lazy[rightNode(node)] += Lazy[node];
            }
            Lazy[node] = 0;
        }
    }
    void build(const vector<long long>& arr, int node, int start, int end) {
        if (start == end) {
            Segment[node] = arr[start];
            return;
        }
        int m = mid(start, end);
        build(arr, leftNode(node), start, m);
        build(arr, rightNode(node), m + 1, end);
        Segment[node] = min(Segment[leftNode(node)], Segment[rightNode(node)]);
    }
    void updateRange_(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end);
        if (r < start || end < l)
            return;
        if (l <= start && end <= r) {
            Lazy[node] += val;
            push(node, start, end);
            return;
        }
        int m = mid(start, end);
        updateRange_(leftNode(node), start, m, l, r, val);
        updateRange_(rightNode(node), m + 1, end, l, r, val);
        Segment[node] = min(Segment[leftNode(node)], Segment[rightNode(node)]);
    }

    long long query_(int node, int start, int end, int l, int r) {
        push(node, start, end);
        if (r < start || end < l)
            return numeric_limits<long long>::max();
        if (l <= start && end <= r)
            return Segment[node];
        int m = mid(start, end);
        long long left_min = query_(leftNode(node), start, m, l, r);
        long long right_min = query_(rightNode(node), m + 1, end, l, r);
        return min(left_min, right_min);
    }

public:
    explicit LazySegmentTree(const vector<long long>& arr) : sz(arr.size()) {
        if (sz > 0) {
            Segment.assign(4 * sz, numeric_limits<long long>::max());
            Lazy.assign(4 * sz, 0);
            build(arr, 0, 0, sz - 1);
        }
    }
    void updateRange(int l, int r, long long val) {
        if (sz > 0 && l <= r) updateRange_(0, 0, sz - 1, l, r, val);
    }
    void update(int idx, long long val) {
        updateRange(idx, idx, val);
    }
    long long query(int l, int r) {
        if (sz == 0 || l > r) return numeric_limits<long long>::max();
        return query_(0, 0, sz - 1, l, r);
    }
};
