class RollbackDSU {
	vector<long long> rank;
	vector<long long> parent;	
	struct Op {
		long long u;
		long long p_u;
		long long v;
		long long r_v;
	};
	stack<Op> history;

public:
	explicit RollbackDSU(long long n) : rank(n), parent(n) {
		for(long long i = 0; i < n; i += 1) {
			parent[i] = i;
			rank[i] = 0;
		}
	}

	long long findSet(long long x) {
		while (x != parent[x]) {
			x = parent[x];
		}
		return x;
	}

	void unionSet(long long x, long long y) {
		long long px = findSet(x);
		long long py = findSet(y);
		if (px == py) return;

		if (rank[px] == rank[py]) {
			history.push({py, parent[py], px, rank[px]});
			parent[py] = px;
			rank[px] += 1;
		}
		else if (rank[px] > rank[py]) {
			history.push({py, parent[py], px, rank[px]});
			parent[py] = px;
		}
		else {
			history.push({px, parent[px], py, rank[py]});
			parent[px] = py;
		}
	}

	long long time() {
		return history.size();
	}

	void rollback(long long target_time) {
		while (history.size() > target_time) {
			Op top = history.top();
			history.pop();
			parent[top.u] = top.p_u;
			rank[top.v] = top.r_v;
		}
	}
};
