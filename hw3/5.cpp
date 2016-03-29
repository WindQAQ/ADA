#include <stdio.h>
#include <vector>
#include <assert.h>

using namespace std;

void larger(int diff, vector <int>& str)
{
	int i = 0;
	bool over = false;
	int size = str.size();

	while(diff){
		if(!over){
			if(diff >= 9 - str[i]){
				diff -= (9 - str[i]);
				str[i] = 9;
			}
			else{
				str[i] += diff;
				diff = 0;
			}
			i++;
			if(i == size)
				over = true;
		}
		else{
			for(int j = 0; j < diff/9; j++)
				str.push_back(9);
			if(diff % 9 != 0)
				str.push_back(diff % 9);
			diff = 0;
		}
	}
	return;
}

void seq(int p, int m, vector <int>& str)
{
	int sum = 0;
	int i = 0;

	while(sum <= m - p)
		sum += str[i++];

	for(int j = i - 1; j >= 0; j--)
		str[j] = 0;

	if(i != str.size())
		str[i]++;
	else
		str.push_back(1);

	for(int j = i; j < str.size() - 1; j++)
		if(str[j] >= 10){
			str[j+1]++;
			str[j] = 0;
		}

	if(str[str.size() - 1] == 10){
		str[str.size() - 1] = 0;
		str.push_back(1);
	}

	sum = 0;

	for(int j = 0; j < str.size(); j++)
		sum += str[j];

	int diff = p - sum;

	for(int j = 0; j < str.size() && diff > 0; j++){
		if(diff >= 9){
			str[j] = 9;
			diff -= 9;
		}
		else{
			str[j] = diff;
			diff = 0;
		}
	}
}

int main()
{
	int T;
	scanf("%d", &T);

	int n;
	while(T--){
		scanf("%d", &n);
		int cur, pre = 0;
		int size;
		vector <int> str;
		str.push_back(0);
		while(n--){
			scanf("%d", &cur);

			if(cur > pre)
				larger(cur - pre, str);
			else
				seq(cur, pre, str);

			size = str.size();

			for(int i = size - 1; i >= 0; i--)
				printf("%d", str[i]);

			int sum = 0;

			for(int i = 0; i < size; i++)
				sum += str[i];

			assert(sum == cur);

			if(n != 0)
				putchar(' ');
			
			pre = cur;
		}
		puts("");
	}
}