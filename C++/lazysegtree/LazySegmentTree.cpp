template <typename T>
class LazySegmentTree {
    vector<T> Segment;
    vector<T> Lazy;
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

    void build(const vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            Segment[node] = arr[start];
            return;
        }
        int m = mid(start, end);
        build(arr, leftNode(node), start, m);
        build(arr, rightNode(node), m + 1, end);
        Segment[node] = min(Segment[leftNode(node)], Segment[rightNode(node)]);
    }

    void updateRange_(int node, int start, int end, int l, int r, T val) {
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

    T query_(int node, int start, int end, int l, int r) {
        push(node, start, end);
        if (r < start || end < l)
            return numeric_limits<T>::max();
        if (l <= start && end <= r)
            return Segment[node];
        int m = mid(start, end);
        T left_min = query_(leftNode(node), start, m, l, r);
        T right_min = query_(rightNode(node), m + 1, end, l, r);
        return min(left_min, right_min);
    }

public:
    explicit LazySegmentTree(const vector<T>& arr) : sz(arr.size()) {
        if (sz > 0) {
            Segment.assign(4 * sz, numeric_limits<T>::max());
            Lazy.assign(4 * sz, 0);
            build(arr, 0, 0, sz - 1);
        }
    }

    void updateRange(int l, int r, T val) {
        if (sz > 0 && l <= r) updateRange_(0, 0, sz - 1, l, r, val);
    }

    void update(int idx, T val) {
        updateRange(idx, idx, val);
    }

    T query(int l, int r) {
        if (sz == 0 || l > r) return numeric_limits<T>::max();
        return query_(0, 0, sz - 1, l, r);
    }
};
