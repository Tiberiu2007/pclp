#include <stdlib.h>
#include <string.h>

char *replace_all(const char *s, const char *vechi, const char *nou) {
    char *out = strdup(s), *seq;
    size_t vechi_len = strlen(vechi), nou_len = strlen(nou);

    seq = strstr(out, vechi);
    while(seq != NULL) {
        memmove(seq + nou_len, seq + vechi_len, strlen(out));
    }
}