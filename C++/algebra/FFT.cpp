class FFT {
    static void bitReversalPermutation(vector<cd>& a) {
        int n = static_cast<int>(a.size());
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
    static void fft(vector<cd>& a, bool invert) {
        int n = a.size();
        bitReversalPermutation(a);
        for (int len = 2; len <= n; len <<= 1) {
            double angle = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(angle), sin(angle));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; ++j) {
                    cd u = a[i + j];
                    cd v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (invert) {
            for (cd& x : a) {
                x /= n;
            }
        }
    }
    static vector<int> multiply(const vector<long long>& a, const vector<long long>& b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < (int)(a.size() + b.size())) {
            n <<= 1;
        }
        fa.resize(n);
        fb.resize(n);
        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; ++i) {
            fa[i] *= fb[i];
        }
        fft(fa, true);
        vector<int> result(n);
        for (int i = 0; i < n; ++i) {
            result[i] = round(fa[i].real());
        }
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        return result;
    }
};
