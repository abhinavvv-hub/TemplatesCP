class NTT {
    static const int MOD = 998244353;
    static const int ROOT = 62;
    static int power(int base, int exp) {
        int res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp >>= 1;
        }
        return res;
    }
    static int modInverse(int n) {
        return power(n, MOD - 2);
    }
    static void ntt_(vector<int>& a) {
        int n = a.size();
        int L = 31 - __builtin_clz(n);
        static vector<int> rt(2, 1);
        for (static int k = 2, s = 2; k < n; k <<= 1, ++s) {
            rt.resize(n);
            int z[] = {1, power(ROOT, MOD >> s)};
            for (int i = k; i < 2 * k; ++i) {
                rt[i] = (rt[i / 2] * z[i & 1]) % MOD;
            }
        }
        vector<int> rev(n);
        for (int i = 0; i < n; ++i) {
            rev[i] = (rev[i / 2] | ((i & 1) << L)) / 2;
        }
        for (int i = 0; i < n; ++i) {
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        }
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; ++j) {
                    int z = (rt[j + k] * a[i + j + k]) % MOD;
                    int& ai = a[i + j];
                    a[i + j + k] = ai - z + (z > ai ? MOD : 0);
                    ai += (ai + z >= MOD ? z - MOD : z);
                }
            }
        }
    }
public:
    static vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        int targetSize = a.size() + b.size() - 1;
        int bits = 32 - __builtin_clz(targetSize);
        int n = 1 << bits;
        int invN = modInverse(n);
        vector<int> fa(a), fb(b), out(n);
        fa.resize(n, 0);
        fb.resize(n, 0);
        ntt_(fa);
        ntt_(fb);
        for (int i = 0; i < n; ++i) {
            out[-i & (n - 1)] = (fa[i] * fb[i]) % MOD * invN % MOD;
        }
        ntt_(out);
        out.resize(targetSize);
        return out;
    }
};
