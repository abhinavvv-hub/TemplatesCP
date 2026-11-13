class WaveletTree {
    int lo, hi;
    WaveletTree *l, *r;
    vector<int> b;

    void build(vector<int>::iterator from, vector<int>::iterator to, int x, int y) {
        lo = x;
        hi = y;
        if (lo == hi || from >= to) return;
        int mid = lo + (hi - lo) / 2;
        auto f = [mid](int val) {
            return val <= mid;
        };
        b.reserve(to - from + 1);
        b.push_back(0);
        for (auto it = from; it != to; ++it) {
            b.push_back(b.back() + f(*it));
        }
        auto pivot = stable_partition(from, to, f);
        l = new WaveletTree();
        l->build(from, pivot, lo, mid);
        r = new WaveletTree();
        r->build(pivot, to, mid + 1, hi);
    }

public:
    WaveletTree() : lo(0), hi(0), l(nullptr), r(nullptr) {}

    WaveletTree(vector<int> arr) {
        if (arr.empty()) return;
        int minVal = *min_element(arr.begin(), arr.end());
        int maxVal = *max_element(arr.begin(), arr.end());
        l = r = nullptr;
        build(arr.begin(), arr.end(), minVal, maxVal);
    }

    int kth(int left, int right, int k) {
        if (left > right) return 0;
        if (lo == hi) return lo;
        int inLeft = b[right] - b[left - 1];
        int lb = b[left - 1];
        int rb = b[right];
        if (k <= inLeft) return l->kth(lb + 1, rb, k);
        return r->kth(left - lb, right - rb, k - inLeft);
    }

    int lte(int left, int right, int k) {
        if (left > right || k < lo) return 0;
        if (hi <= k) return right - left + 1;
        int lb = b[left - 1], rb = b[right];
        return l->lte(lb + 1, rb, k) + r->lte(left - lb, right - rb, k);
    }

    int count(int left, int right, int k) {
        if (left > right || k < lo || k > hi) return 0;
        if (lo == hi) return right - left + 1;
        int lb = b[left - 1], rb = b[right], mid = lo + (hi - lo) / 2;
        if (k <= mid) return l->count(lb + 1, rb, k);
        return r->count(left - lb, right - rb, k);
    }
    
    int query(int left, int right, int mn) {
        if (left > right || (right - left + 1) <= mn) return -1;
        if (lo == hi) return lo;
        int lb = b[left - 1];
        int rb = b[right];
        int inLeft = rb - lb;
        if (inLeft > mn) {
            int res = l->query(lb + 1, rb, mn);
            if (res != -1) return res;
        }
        int inRight = (right - left + 1) - inLeft;
        if (inRight > mn) {
            return r->query(left - lb, right - rb, mn);
        }
        return -1;
    }

    ~WaveletTree() {
        delete l;
        delete r;
    }
};
