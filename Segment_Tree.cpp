#include <bits/stdc++.h>
using namespace std;

template <typename T>
class SegmentTree {
    vector<T> Segment;
    vector<T> array;
    int sz;

    int leftNode(int n) { return 2 * n + 1; }
    int rightNode(int n) { return 2 * n + 2; }
    int mid(int l, int r) { return l + (r - l) / 2; }

    void build(int node, int start, int end) {
        if (start == end) {
            Segment[node] = array[start];
            return;
        }
        int m = mid(start, end);
        build(leftNode(node), start, m);
        build(rightNode(node), m + 1, end);
        Segment[node] = min(Segment[leftNode(node)], Segment[rightNode(node)]);
    }

    void update_(int node, int start, int end, int idx, T val) {
        if (start == end) {
            array[idx] = val;
            Segment[node] = val;
            return;
        }
        int m = mid(start, end);
        if (idx <= m)
            update_(leftNode(node), start, m, idx, val);
        else
            update_(rightNode(node), m + 1, end, idx, val);
        Segment[node] = min(Segment[leftNode(node)], Segment[rightNode(node)]);
    }

    T query_(int node, int start, int end, int l, int r) {
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
    explicit SegmentTree(vector<T> &arr) : array(arr), sz(arr.size()) {
        Segment.assign(4 * sz, numeric_limits<T>::max());
        build(0, 0, sz - 1);
    }

    void update(int idx, T val) {
        update_(0, 0, sz - 1, idx, val);
    }

    T query(int l, int r) {
        return query_(0, 0, sz - 1, l, r);
    }
};
