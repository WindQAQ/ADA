#include <cstdio>
#include <cstdlib>

#define MAX 200010

int main()
{
	int T;
	scanf("%d", &T);

	int n;
	int arr[MAX];
	long long int e, c, p;
	int mid, right_end, cur_size, left_start, ii, jj;
	int n1, n2;
	long long int result;
	long long int power, base, in;
	int i, j, k;
	for(ii = 0; ii < T; ii++){
		scanf("%d%lld%lld%lld", &n, &c, &e, &p);
		e %= (p-1);
		
		for(jj = 0; jj < n; jj++)
			arr[jj] = jj+1;

		int *table = (int*) calloc((2*n+10), sizeof(int));
		
		for(int x = 2; x <= 3; x++){
			in = e;
			base = x;	
			power = 1;
			while(in > 0){
				if(in & (long long int)1)
					power = (power*base) % p;
				base = (base*base) % p;
				in >>= 1;
			}
			power %= p;
			table[x] = (int)power;
		}

		for(int x = 4; x <= 2*n; x++){
			if(x % 2 == 0){
				table[x] = (long long)table[2]*table[x/2] % p;
				continue;
			}
			else if(x % 3 == 0){
				table[x] = (long long)table[3]*table[x/3] % p;
				continue;
			}
			else{
				in = e;
				base = x;
				power = 1;
				while(in > 0){
					if(in & (long long int)1)
						power = (power*base) % p;
					base = (base*base) % p;
					in >>= 1;
				}
				power %= p;
				table[x] = (int)power;
			}
		}

		for(cur_size = 1; cur_size <= n-1; cur_size *= 2)
			for(left_start = 0; left_start < n-1; left_start += 2*cur_size){

				mid = left_start + cur_size - 1;
				right_end = (left_start + 2*cur_size -1 < n-1)?
							 left_start + 2*cur_size -1: n-1;
				
				if(mid > n-1) mid = n-1;
				
				n1 = mid - left_start + 1;
			    n2 = right_end - mid;

				int* left = (int*)malloc(sizeof(int)*n1);
				int* right = (int*)malloc(sizeof(int)*n2);
				
				for(i = 0; i < n1; i++)
					left[i] = arr[left_start+i];

				for(j = 0; j < n2; j++)
					right[j] = arr[mid+1+j];

				i = 0; j = 0; k = left_start;

				while(i < n1 && j < n2){
					power = table[left[i]+right[j]];

					result = (((c*(left[i]-right[j]))%p)*power)%p;
					result = (result >= 0)? result: result+p;

					if((double)result > (double)p/2)
						arr[k++] = left[i++];
					else
						arr[k++] = right[j++];
				}

				while(i < n1)
					arr[k++] = left[i++];
				while(j < n2)
					arr[k++] = right[j++];
				free(left);
				free(right);
			}
		free(table);
		for(jj = 0; jj < n; jj++)
			printf("%d ", arr[jj]);

		printf("\n");
	}

	return 0;
}
