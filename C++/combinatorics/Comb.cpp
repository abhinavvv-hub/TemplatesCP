class Comb {
public:
    vector<int> fact, invfact;
    int MOD, N;

    Comb(int n = 200005, int mod = 1000000007) : N(n), MOD(mod) {
        fact.resize(N);
        invfact.resize(N);
        precompute();
    }

    int mod_pow(int base, int exp) {
        int res = 1;
        base %= MOD;
        while (exp > 0) {
            if (exp & 1) res = res * base % MOD;
            base = base * base % MOD;
            exp >>= 1;
        }
        return res;
    }

    void precompute() {
        fact[0] = 1;
        for (int i = 1; i < N; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        invfact[N - 1] = mod_pow(fact[N - 1], MOD - 2);
        for (int i = N - 2; i >= 0; i--) {
            invfact[i] = invfact[i + 1] * (i + 1) % MOD;
        }
    }

    int binom(int n, int r) {
        if (r < 0 || n < r || n < 0) return 0;
        return fact[n] * invfact[r] % MOD * invfact[n - r] % MOD;
    }

    int perm(int n, int r) {
        if (r < 0 || n < r || n < 0) return 0;
        return fact[n] * invfact[n - r] % MOD;
    }

    int inv(int x) {
        return mod_pow(x, MOD - 2);
    }
};
