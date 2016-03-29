#include <stdio.h>
#include <string.h>

#define MAX 25

typedef long long LL;

LL dp[19][19][19][7] = {{{{0}}}};
bool use[19][19][19][7] = {{{{0}}}};

LL f(int digit, int cnt4, int cnt7, int mod7)
{
	if(digit == 0){
		if((cnt7 >= 3) && (cnt7 > cnt4) && (mod7 % 7 == 0))
			return 1;
		return 0;
	}
	if(use[digit][cnt4][cnt7][mod7])
		return dp[digit][cnt4][cnt7][mod7];

	use[digit][cnt4][cnt7][mod7] = true;
	LL result = 0;
	for(int i = 0; i <= 9; i++)
		result += f(digit-1, (i == 4)? cnt4+1: cnt4, (i == 7)? cnt7+1: cnt7, (mod7*10+i)%7);
	dp[digit][cnt4][cnt7][mod7] = result;
	return result;
}

LL lucky(LL a)
{
	LL result = 0;
	char s[MAX];
	sprintf(s, "%lld", a);
	int len = strlen(s);
	int remain = len;
	int cnt4 = 0, cnt7 = 0, mod7 = 0;

	for(int i = 0; i < len; i++){
		remain--;
		int d = s[i] - '0';
		for(int j = 0; j < d; j++)
			result += f(remain, (j == 4)? cnt4+1: cnt4, (j == 7)? cnt7+1: cnt7, (mod7*10+j)%7);
		if(d == 4)
			cnt4++;
		if(d == 7)
			cnt7++;
		mod7 = (mod7*10 + d) % 7;
	}
	
	return result;
}

int main()
{
	int T;
	scanf("%d", &T);
	LL a, b;

	while(T--){
		scanf("%lld%lld", &a, &b);
		printf("%lld\n", lucky(b+1) - lucky(a));
	}

	return 0;
}