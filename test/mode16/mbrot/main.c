#include <stdio.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define MAX_ITERATIONS 20
#define ESCAPE_RADIUS 2

#define FRACTION_BITS 8
#define FRACTION_SIZE (1 << FRACTION_BITS)

#define TO_FIXED(x) (x * FRACTION_SIZE)
#define FROM_FIXED(x) (x / FRACTION_SIZE)
#define ADD_FIXED(x, y) (x + y)
#define SUB_FIXED(x, y) (x - y)
#define MUL_FIXED(x, y) ((x * y) >> FRACTION_BITS)
#define DIV_FIXED(x, y) ((x << FRACTION_BITS) / y)

long mbrot(long x, long y, long zoom, long offs_x, long offs_y)
{
    long i = 0;
    long zx = 0, zy = 0;
    long new_zx, new_zy;

    long cx = TO_FIXED(SCREEN_WIDTH) >> 1;
    long cy = TO_FIXED(SCREEN_HEIGHT) >> 1;

    if (cx)
    {
        // printf("AHOJ %d\n", cx);
    }

    cx = SUB_FIXED(TO_FIXED(x), cx);
    cy = SUB_FIXED(TO_FIXED(y), cy);

    if (cx)
    {
        // printf("AHOJ2 %d\n", cx);
    }

    // cx = DIV_FIXED(cx, zoom) + offs_x;
    // cy = DIV_FIXED(cy, zoom) + offs_y;

    cx = cx << FRACTION_BITS;
    cy = cy << FRACTION_BITS;

    // printf("AHOJ3 %d\n", cx);

    cx = cx / zoom;
    cy = cy / zoom;

    if (cx)
    {
        // printf("AHOJ4 %d\n", cx);
    }

    long cmp1, cmp2, left, right;
    cmp1 = i < MAX_ITERATIONS && (MUL_FIXED(zx, zx) + MUL_FIXED(zy, zy) < TO_FIXED(ESCAPE_RADIUS * ESCAPE_RADIUS));

    for (i = 0; cmp1; )
    {
        new_zx = ADD_FIXED(SUB_FIXED(MUL_FIXED(zx, zx), MUL_FIXED(zy, zy)), cx);
        new_zy = ADD_FIXED(MUL_FIXED(TO_FIXED(2), MUL_FIXED(zx, zy)), cy);

        zx = new_zx;
        zy = new_zy;

        i++;
        cmp1 = i < MAX_ITERATIONS && (MUL_FIXED(zx, zx) + MUL_FIXED(zy, zy) < TO_FIXED(ESCAPE_RADIUS * ESCAPE_RADIUS));
    }

    return i;
}

long main(void)
{
    long x, y, i;
    long zoom = TO_FIXED(SCREEN_WIDTH);

    while (1)
    {
        system("clear");
        printf("%lld\n", zoom);
        for (y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (x = 0; x < SCREEN_WIDTH; x++)
            {
                i = mbrot(x, y, zoom, 0, 0);

                if (i == MAX_ITERATIONS)
                {
                    putchar('#');
                }
                else
                {
                    putchar('.');
                }
            }
            putchar('\n');
        }

        switch (getchar())
        {
        case '+':
            zoom += 500;
            break;
        case '-':
            zoom -= 500;
            break;
        }
    }
    return 0;
}