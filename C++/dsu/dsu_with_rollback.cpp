class RollbackDSU {
	vector<long long> rank;
	vector<long long> parent;	
	long long components; 
	
	struct Op {
		long long u;
		long long p_u;
		long long v;
		long long r_v;
	};
	stack<Op> history;

public:
	explicit RollbackDSU(long long n) : rank(n), parent(n), components(n) {
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

		components -= 1;

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
			
			components += 1;
		}
	}

	long long getComponents() {
		return components;
	}
};


template <typename T>
class SegmentTreeTime {
	vector<vector<pair<T, T>>> Segment;
	int sz;

	int leftNode(int n) { return 2 * n + 1; }
	int rightNode(int n) { return 2 * n + 2; }
	int mid(int l, int r) { return l + (r - l) / 2; }

	void insert_(int node, int start, int end, int l, int r, pair<T, T> edge) {
		if (r < start || end < l)
			return;
		if (l <= start && end <= r) {
			Segment[node].push_back(edge);
			return;
		}
		int m = mid(start, end);
		insert_(leftNode(node), start, m, l, r, edge);
		insert_(rightNode(node), m + 1, end, l, r, edge);
	}

	void solve_(int node, int start, int end, RollbackDSU &dsu, vector<T> &ans) {
		long long t = dsu.time();

		for (auto &edge : Segment[node]) {
			dsu.unionSet(edge.first, edge.second);
		}

		if (start == end) {
			ans[start] = dsu.getComponents();
		} 
		else {
			int m = mid(start, end);
			solve_(leftNode(node), start, m, dsu, ans);
			solve_(rightNode(node), m + 1, end, dsu, ans);
		}

		dsu.rollback(t);
	}

public:
	explicit SegmentTreeTime(int total_queries) : sz(total_queries) {
		Segment.resize(4 * sz);
	}

	void insert(int l, int r, pair<T, T> edge) {
		if (l > r) return;
		insert_(0, 0, sz - 1, l, r, edge);
	}

	vector<T> solve(RollbackDSU &dsu) {
		vector<T> ans(sz);
		solve_(0, 0, sz - 1, dsu, ans);
		return ans;
	}
};
