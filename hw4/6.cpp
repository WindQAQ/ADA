#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#define MAX_E 100050
#define MAX_V 50050

using namespace std;

typedef struct edge{
	int v1, v2, w, i;
} Edge;

vector <int> adj[MAX_V];
Edge E[MAX_E];
int p[MAX_V], r[MAX_V];
bool useless[MAX_E];
int dfs[MAX_V] = {0}, low[MAX_V], VtoV[MAX_E];
int stamp, ret, GW;
long long ret_cost;

int find_set(int x)
{
	return (x == p[x])? x: (p[x] = find_set(p[x]));
}

void union_set(int a, int b)
{
	int x = find_set(a), y = find_set(b);
	if(r[x] > r[y])
		p[y] = x;
	else{
		p[x] = y;
		if(r[x] == r[y])
			r[y]++;
	}
}

void init(int n)
{
	ret = 0;
	ret_cost = 0;
	memset(useless, false, sizeof(useless));
	for(int i = 0; i < n; i++){
		p[i+1] = i+1;
		r[i+1] = 0;
	}
}

void tarjan(int u)
{
	dfs[u] = low[u] = ++stamp;
	for(int i = 0; i < adj[u].size(); i++){
		int j = adj[u][i];
		if(!useless[j]){
			useless[j] = true;
			int v = VtoV[j] - u;
			if(dfs[v])
				low[u] = min(low[u], dfs[v]);
			else{
				tarjan(v);
				low[u] = min(low[u], low[v]);
				if(low[v] > dfs[u]){
					ret++;
					ret_cost += GW;
				}
			}
		}
	}
}

int main()
{
	int T;
	scanf("%d", &T);

	while(T--){
		int n, m;
		scanf("%d%d", &n, &m);
		for(int i = 0; i < m; i++){
			scanf("%d%d%d", &E[i].v1, &E[i].v2, &E[i].w);
			E[i].i = i;
		}

		sort(E, E + m, [](const Edge& a, const Edge& b)->bool{return a.w < b.w;});

		init(n);

		for(int i = 0; i < m; i++){
			
			int w = E[i].w, index = i;
			while(index < m && E[index].w == w){
				GW = w;
				int p1 = find_set(E[index].v1), p2 = find_set(E[index].v2);
				if(p1 != p2){
					adj[p1].push_back(E[index].i);
					adj[p2].push_back(E[index].i);
					VtoV[E[index++].i] = p1 + p2;
				}
				else useless[E[index++].i] = true;
			}

			for(int j = i; j < index; j++)
				if(!useless[E[j].i]){
					stamp = 0;
					tarjan(p[E[j].v1]);
				}

			/* clear */
			for(int j = i; j < index; j++){
				int p1 = p[E[j].v1], p2 = p[E[j].v2];
				dfs[p1] = dfs[p2] = 0;
				adj[p1].clear();
				adj[p2].clear();
			}

			for(int j = i; j < index; j++)
				union_set(E[j].v1, E[j].v2);

			i = index - 1;
		}
		printf("%d %lld\n", ret, ret_cost);
	}
	return 0;
}