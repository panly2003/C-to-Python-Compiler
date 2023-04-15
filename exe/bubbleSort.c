int a[100];

int main()
{
    int num;
    printf("Input the array length: ");
    scanf("%d", &num);
    printf("Input integers: ");
    for (int i = 0; i < num; i = i + 1)
    {
        scanf("%d", &a[i]);
    }
    for (int i = num - 1; i > 0; i = i - 1)
    {
        for (int j = 0; j < i; j = j + 1)
        {
            if (a[j] > a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    printf("Result: ");
    for (int i = 0; i < num; i = i + 1)
    {
        printf("%d ", a[i]);
    }

    return 0;
}