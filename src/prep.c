#include "includes.h"

int __TMP_frontend_main(int argc, char *argv[], FILE *outfile);

void prep(void)
{
    FILE *tmp = tmpfile();

    char **list[2];
    list[1] = src_n;

    __TMP_frontend_main(2, list, tmp);

    fseek(tmp, 0, SEEK_SET);
    src_f = tmp;
}
