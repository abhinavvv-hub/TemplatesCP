/*
First Fill `fact` array to pre compute factoial
before any test case and define the pow_mod function to fastly compute power using binomial exponentiation.
How To Use the Functions
*/
int num_of_topo_ordering_of_tree(int n, vector<vector<int>> &adj) {
    long long subProd = 1ll;
    auto dfs = [&](int u, int p, auto &&F) -> long long {
            long long currSz = 1;
            for (int v : adj[u]) {
                if (v != p) {
                    currSz += F(v, u, F);
                }
            }
            subProd = (subProd * currSz) % MOD;
            return currSz;
    };
    dfs(0, -1, dfs);
    return (fact[n] * pow_mod(subProd, mod - 2)) % MOD;
}
