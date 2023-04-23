#include "includes.h"

char *nofile = "ERROR: no %s file specified\n"
               "use the -h or --help flag for help\n";

char *usage = "usage: %s [flags] <input file> <output file>\n\n"
              "flags:\n"

              "\t-h, --help display this help message\n\n"

              "\t-m <mode>\n"
              "\t    64\t\t64-bit mode (DEFAULT)\n"
              "\t    16\t\t16-bit mode\n\n"

              "\t-cc <calling convention>\n"
              "\t     sysv\tSystemV (Linux, most Unix variants)\n"
              "\t     ms64\tMicrosoft x64 (Windows)\n";

int flags(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
        if (argv[i][0] != '-')
        {
            return i;
        }

        /* HELP */
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            printf(usage, argv[0]);
            exit(0);
        }

        /* CALLING CONVENTION */
        if (!strncmp(argv[i], "-cc", strlen("-cc")))
        {
            /* FLAG AND ARGUMENT TOGETHER */
            if (!strcmp(argv[i] + strlen("-cc"), "sysv"))
            {
                ALIGN = 16;
                arg64 = elf64;
                i += 1;
                continue;
            }

            if (!strcmp(argv[i] + strlen("-cc"), "ms64"))
            {
                ALIGN = 32;
                arg64 = win64;
                i += 1;
                continue;
            }

            if (argc < (i + 2))
            {
                goto cc_end;
            }

            /* FLAG AND ARGUMENT SEPARATE */
            if (!strcmp(argv[i + 1], "sysv"))
            {
                ALIGN = 16;
                arg64 = elf64;
                i += 2;
                continue;
            }

            if (!strcmp(argv[i + 1], "ms64"))
            {
                ALIGN = 32;
                arg64 = win64;
                i += 2;
                continue;
            }

        cc_end:
            printf("ERROR: unknown calling convention\n");
            exit(-1);
        }

        /* MODE */
        if (!strncmp(argv[i], "-mode", strlen("-mode")))
        {
            /* FLAG AND ARGUMENT TOGETHER */
            if (!strcmp(argv[i] + strlen("-mode"), "16"))
            {
                mode(M_16);
                continue;
            }

            if (!strcmp(argv[i] + strlen("-mode"), "64"))
            {
                mode(M_64);
                continue;
            }

            if (argc < (i + 2))
            {
                goto mode_end;
            }

            /* FLAG AND ARGUMENT SEPARATE */
            if (!strcmp(argv[i + 1], "16"))
            {
                mode(M_16);
                continue;
            }

            if (!strcmp(argv[i + 1], "64"))
            {
                mode(M_64);
                continue;
            }

        mode_end:
            printf("ERROR: unknown mode\n");
            exit(-1);
        }

        printf("ERROR: unknown flag %s\n", argv[i]);
        exit(-1);
    }

    return i;
}

void input(int argc, char **argv)
{
    int i;

    if (argc == 1)
    {
        printf(nofile, "input");
        exit(-1);
    }

    mode(M_64);
    i = flags(argc, argv);

    if (!access(argv[i], F_OK))
    {
        src_n = argv[i];
    }
    else
    {
        printf(nofile, "input");
        exit(-1);
    }

    i++;

    if (i < argc)
    {
        out_n = argv[i];
    }
    else
    {
        printf(nofile, "output");
        exit(-1);
    }
}
