#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *replace_all(const char *s, const char *vechi, const char *nou) {
    size_t s_len = strlen(s), vechi_len = strlen(vechi), nou_len = strlen(nou);

    const char *ptr = strstr(s, vechi);
    int count = 0;
    while(ptr != NULL) {
        count++;
        ptr = strstr(ptr + 1, vechi);
    }

    int result_len = s_len - count * vechi_len + count * nou_len;
    char *result = malloc(result_len + 1);
    char *out = result;
    const char *cur = s;

    ptr = strstr(cur, vechi);
    while(ptr != NULL) {
        size_t prefix_len = (size_t)(ptr - cur);
        strncpy(out, cur, prefix_len);
        out += prefix_len;
        strncpy(out, nou, nou_len);
        out += nou_len;
        cur = ptr + vechi_len;

        ptr = strstr(cur, vechi);
    }
    strcpy(out, cur);

    return result;
}

int main(void) {
    const char str[] = "Ana are mere";

    char *out = replace_all(str, "a", "AA");

    printf("%s\n", out);

    return 0;
}