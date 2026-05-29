#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX   (uint32_t)1e4 + 10
#define BASE  2
#define PRIME 233317

uint32_t n;
uint64_t v[MAX] = {0};
char str[MAX][1510];

int cmp(const void* p_a, const void* p_b)
{
    return *(uint64_t*)p_a < *(uint64_t*)p_b;
}

int main()
{
    scanf("%u", &n);
    for (uint32_t i = 0u; i < n; ++i) {
        char* it = &str[i][0];
        scanf("%s", it);
        while (*it != '\0') {
            v[i] += ((v[i] << 1) + *it) % UINT32_MAX + PRIME;
            ++it;
        }
    }
    qsort(v, n, sizeof(uint64_t), cmp);
    uint32_t ans = 1u;
    for (uint32_t i = 1u; i < n; ++i) {
        ans += (v[i] != v[i - 1]);
    }
    printf("%u\n", ans);
    return 0;
}
