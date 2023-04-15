char a[100];

int main() {
    int num;
    printf("Input the string length: ");
    scanf("%d", &num);
    printf("Input string: ");
    scanf("%s", a);
    int half = num / 2;
    for (int i = 0; i < half; i = i + 1) {
        int x = num - 1;
        int a1 = a[i];
        int a2 = a[x - i];
        if (a1 != a2) {
            printf("FALSE!");
            return 0;
        }
    }
    printf("TRUE!");
    return 0;
}