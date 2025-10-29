#include <bits/stdc++.h>
using namespace std;

template <typename T>
class SegmentTree {
    vector<T> Segment;
    vector<T> array;
    int sz;
    int leftNode(int n) {
        return 2 * n + 1;
    }
    int rightNode(int n) {
        return 2 * n + 2;
    }
    int mid(int l, int r) { 
        return l + (r - l) / 2; 
    }
    void build(int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int m = mid(start, end);
        build(left(node), start, m);
        build(right(node), m + 1, end);
        tree[node] = min(tree[left(node)], tree[right(node)]);
    }
    void update_(int node, int start, int end, int idx, T val) {
        if (start == end) {
            arr[idx] = val;
            tree[node] = val;
            return;
        }
        int m = mid(start, end);
        if (idx <= m) {
            update_(left(node), start, m, idx, val);
        }
        else {
            update_(right(node), m + 1, end, idx, val);
        }
        tree[node] = min(tree[left(node)], tree[right(node)]);
    }
    T query_(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return INT_MAX;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int m = mid(start, end);
        T left_min = query_(left(node), start, m, l, r);
        T right_min = query_(right(node), m + 1, end, l, r);
        return min(left_min, right_min);
    }
public:
    explicit SegmentTree(vector<T> &arr) const : array(arr), sz(arr.size()){
        Segment.resize(4 * sz + 1);
        build(0, 0, sz - 1);
    }
    void update(int idx, T val) {
        update_(0, 0, n - 1, idx, val);
    }
    T query(int l, int r) {
        return query_(0, 0, n - 1, l, r);
    }
};
