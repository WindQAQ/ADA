#include <stdio.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

#define MAXV 10100
#define MAXE 100010

using namespace std;

typedef struct vertex{
	int id, self, degree;
} Vertex;

typedef struct edge{
	int v1, v2;

	void set(int a, int b){
		v1 = a;
		v2 = b;
	}

} Edge;

typedef unordered_map <int, unordered_map <int, int> >::iterator myiterator;

int n, m;
int assignment[MAXV];
int used[MAXV];
Vertex VG[MAXV], VH[MAXV];
Edge EG[MAXE], EH[MAXE];
unordered_map <int, unordered_map <int, int> > adjG, adjH;

void init()
{
	memset(used, 0, sizeof(used));
	memset(VG, 0, sizeof(VG));
	memset(VH, 0, sizeof(VH));
	memset(assignment, 0, sizeof(assignment));
	adjG.clear();
	adjH.clear();
}

bool sub(int current, int p)
{
	if(adjG[current].size() != adjH[p].size())
		return false;

	for(unordered_map <int, int>::iterator it = adjG[current].begin(); it != adjG[current].end(); it++){
		int x = it->first;
		if(x <= current){
			myiterator hit;
			if((hit = adjH.find(assignment[x])) != adjH.end() && hit->second.find(p) != hit->second.end()){
				if(adjG[current][x] != adjH[p][assignment[x]])
					return false;
			}
			else return true;
		}
	}

	return true;
}

void refine(vector <int> candidate[MAXV])
{
	// printf("eqwwqe\n");
	bool change = true;
	while(change){
		change = false;
		for(int i = 1; i <= n; i++){
			vector <int> temp(candidate[i]);
			for(int j = 0; j < temp.size(); j++){
				int x = temp[j];
				for(unordered_map <int, int>::iterator it = adjG[i].begin(); it != adjG[i].end(); it++){
					bool match = false;
					for(int k = 0; k < candidate[it->first].size(); k++){
						int y = candidate[it->first][k];
						// printf("%d -> %d, %d -> %d, it->second = %d, adjH[x][y] = %d\n", i, x, it->first, y, it->second, adjH[x][y]);
						myiterator hit;
						if((hit = adjH.find(x)) != adjH.end() && (hit->second.find(y) != hit->second.end()) && it->second == adjH[x][y]){
							// printf("true!\n");
							match = true;
						}
						// else printf("false\n");
					}
					if(!match){
						for(int k = 0; k < candidate[i].size(); k++)
							if(candidate[i][k] == x){
								candidate[i].erase(candidate[i].begin()+k);
								break;
							}
						change = true;
					}
				}
			}
		}
	}
}

bool search(int current, vector <int> candidate[MAXV])
{
	// for(int i = 1; i < current; i++)
	// 	printf("%d ", assignment[i]);
	// printf("\n");
	if(current == n + 1){
		// printf("qwqw\n");
		printf("%d", assignment[1]);
		for(int i = 2; i <= n; i++)
			printf(" %d", assignment[i]);
		printf("\n");
		return true;
	}

	refine(candidate);

	vector <int> temp(candidate[current]);
	for(int i = 0; i < temp.size(); i++){
		int p = temp[i];
		if(!sub(current, p)) continue;
		if(used[p] != 0) continue;
		vector <int> new_candidate[MAXV];
		for(int j = 1; j <= n; j++)
			for(int k = 0; k < candidate[j].size(); k++)
				new_candidate[j].push_back(candidate[j][k]);
		new_candidate[current].clear();
		new_candidate[current].push_back(p);
		used[p] = current;
		assignment[current] = p;
		if(search(current + 1, new_candidate))
			return true;
		used[p] = 0;
		assignment[current] = 0;
	}

	return false;
}

int main()
{
	int T;
	scanf("%d", &T);
	myiterator it;
	while(T--){
		scanf("%d%d", &n, &m);

		init();

		for(int i = 0; i < m; i++){
			int a, b;
			scanf("%d%d", &a, &b);
			EG[i].set(a, b);
			if((it = adjG.find(a)) != adjG.end() && (it->second.find(b) != it->second.end()))
				adjG[a][b]++;
			else adjG[a][b] = 1;
			if((it = adjG.find(b)) != adjG.end() && (it->second.find(a) != it->second.end()))
				adjG[b][a]++;
			else adjG[b][a] = 1;
			VG[a].id = a;
			VG[b].id = b;
			VG[a].degree++;
			VG[b].degree++;
			if(a == b) VG[a].self++;
		}

		for(int i = 0; i < m; i++){
			int a, b;
			scanf("%d%d", &a, &b);
			EH[i].set(a, b);
			if((it = adjH.find(a)) != adjH.end() && (it->second.find(b) != it->second.end()))
				adjH[a][b]++;
			else adjH[a][b] = 1;
			if((it = adjH.find(b)) != adjH.end() && (it->second.find(a) != it->second.end()))
				adjH[b][a]++;
			else adjH[b][a] = 1;
			VH[a].id = a;
			VH[b].id = b;
			VH[a].degree++;
			VH[b].degree++;
			if(a == b) VH[a].self++;
		}

		int eigenG[MAXV], eigenH[MAXV], copyG[MAXV], copyH[MAXV];
		memset(eigenG, 0, sizeof(eigenG));
		memset(eigenH, 0, sizeof(eigenH));
		for(int i = 1; i <= n; i++){
			copyG[i] = VG[i].degree;
			copyH[i] = VH[i].degree;
		}

		for(int i = 0; i < 12; i++){
			for(int j = 0; j < m; j++){
				eigenG[EG[j].v1] += copyG[EG[j].v2];
				eigenG[EG[j].v2] += copyG[EG[j].v1];
				eigenH[EH[j].v1] += copyH[EH[j].v2];
				eigenH[EH[j].v2] += copyH[EH[j].v1];
			}
			memcpy(copyG, eigenG, sizeof(eigenG));
			memcpy(copyH, eigenH, sizeof(eigenH));
			// for(int j = 1; j <= n; j++)
			// printf("eigen of %d: %d\n", j, eigenG[j]);
		}

		// for(int i = 1; i <= n; i++)
		// 	printf("eigen of %d: %d\n", i, eigenG[i]);

		vector <int> candidate[MAXV];

		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= n; j++){
				if(VG[i].degree == VH[j].degree && VG[i].self == VH[j].self && eigenG[i] == eigenH[j])
					candidate[i].push_back(j);
			}

		// for(int i = 1; i <= n; i++){
		// 	printf("candidate of %d:", i);
		// 	for(int j = 0; j < candidate[i].size(); j++)
		// 		printf(" %d", candidate[i][j]);
		// 	printf("\n");
		// }

		// printf("===========\n");
		// for(myiterator it = adjG.begin(); it != adjG.end(); it++)
		// 	for(unordered_map <int, int>::iterator lt = it->second.begin(); lt != it->second.end(); lt++)
		// 		printf("Edge: (%d, %d) : %d\n", it->first, lt->first, lt->second);
		// printf("===========\n");

		search(1, candidate);
	}

	return 0;
}