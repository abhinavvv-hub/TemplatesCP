class NTT {
    static const int MOD = 998244353;
    static const int G = 3;
    static int power(int base, int exp) {
        int res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }
    static long long modInverse(long long n) {
        return power(n, MOD - 2);
    }
    static void bitReversalPermutation(vector<long long>& a) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) {
                j ^= bit;
            }
            j ^= bit;
            if (i < j) {
                swap(a[i], a[j]);
            }
        }
    }
public:
    static void ntt(vector<int>& a, bool invert) {
        int n = static_cast<int>(a.size());
        bitReversalPermutation(a);
        for (int len = 2; len <= n; len <<= 1) {
            int wlen = power(G, (MOD - 1) / len);
            if (invert) {
                wlen = modInverse(wlen);
            }
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; ++j) {
                    long long u = a[i + j];
                    long long v = (a[i + j + len / 2] * w) % MOD;
                    a[i + j] = (u + v >= MOD ? u + v - MOD : u + v);
                    a[i + j + len / 2] = (u - v < 0 ? u - v + MOD : u - v);
                    w = (w * wlen) % MOD;
                }
            }
        }
        if (invert) {
            long long n_inv = modInverse(n);
            for (long long& x : a) {
                x = (x * n_inv) % MOD;
            }
        }
    }
    static vector<int> multiply(const vector<long long>& a, const vector<long long>& b) {
        vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < (int)(a.size() + b.size())) {
            n <<= 1;
        }
        fa.resize(n, 0);
        fb.resize(n, 0);
        ntt(fa, false);
        ntt(fb, false);
        for (int i = 0; i < n; ++i) {
            fa[i] = (fa[i] * fb[i]) % MOD;
        }
        ntt(fa, true);
        while (fa.size() > 1 && fa.back() == 0) {
            fa.pop_back();
        }
        return fa;
    }
};
