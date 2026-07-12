class DSU {
	vector<long long> rank;
	vector<long long> parent;
public:
	explicit DSU(long long n) : rank(n), parent(n) {
		for(long long i = 0; i < n; i += 1) {
			parent[i] = i;
			rank[i] = 0;
		}
	}
	long long findSet(long long x) {
		if (parent[x] == x) return x;
		return parent[x] = findSet(parent[x]);
	}
	void unionSet(long long x, long long y) {
		long long px = findSet(x);
		long long py = findSet(y);
		if (px == py) return;
		if (rank[px] == rank[py]) {
			parent[py] = px;
			rank[px] += 1;
		}
		else if (rank[px] > rank[py]) {
			parent[py] = px;
		}
		else {
			parent[px] = py;
		}
	}
};
