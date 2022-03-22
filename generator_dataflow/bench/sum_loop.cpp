// __cgra__
int sum_loop(int *a, int n) {
	int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
}

int main() {
	int n = 100;
    int a[n];

    for (int i = 0; i < n; ++i) {
        a[i] = i + 1;
    }

	int c = sum_loop(a, n);

	printf("sum = %d\n", c);

	return 0;
}
