#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct element
{
    long double x;
    long double y;
    long double deg;
    long double mag;
};

struct vector
{
    size_t capacity;
    size_t size;
    struct element* array;
};

int insert(struct vector* ptr);

int main(int argc, char** argv)
{
    struct vector data;
    int idx;
    long double x = 0;
    long double y = 0;
    long double deg;
    if(argc > 1)
    {
        data.capacity = atoi(argv[1]);
        data.array = (struct element*) malloc(data.capacity * sizeof(struct element));
        
        for(idx = 0; idx < data.capacity; ++idx)
        {
            if(insert(&data) == 0)
            {
                break;
            }
        }
    }
    else
    {
        data.capacity = 8;
        data.size = 0;
        data.array = (struct element*) malloc(data.capacity * sizeof(struct element));

        while(insert(&data));
    }

    fprintf(stdout, "\n X Magnitude    Y Magnitude     Magnitude      Direction\n");
    fprintf(stdout, "-------------  -------------  -------------  -------------\n");
    for(idx = 0; idx < data.size; ++idx)
    {
        x += data.array[idx].x;
        y += data.array[idx].y;
        fprintf(stdout, "%13.5Lf  %13.5Lf  %13.5Lf  %13.5Lf\n", data.array[idx].x, data.array[idx].y, data.array[idx].mag, data.array[idx].deg);
    }
    fprintf(stdout, "-------------  -------------  -------------  -------------\n");
    deg = atan2l(y, x) * 180 / M_PI;
    if(deg < 0)
    {
        deg += 360.0;
    }
    fprintf(stdout, "%13.5Lf  %13.5Lf  %13.5Lf  %13.5Lf\n\n", x, y, (hypotl(x, y)), deg);

    return 0;
}

int insert(struct vector* ptr)
{
    static int count = 1;
    int ret = 0;
    char buf[80];
    char* next;
    long double first;

    fprintf(stdout, "%i:>> ", count++);
    fgets(buf, 80, stdin);

    if((strcmp(buf,"\n")) != 0 && (strcmp(buf, "quit\n") != 0))
    {
        if(ptr->capacity == ptr->size)
        {
            ptr->capacity <<= 1;
            ptr->array = (struct element*) realloc(ptr->array, ptr->capacity * sizeof(struct element));
        }
        first = strtold(buf, &next);
        while(isblank(*next))
        {
            ++next;
        }

        if(*next == ',')
        {
            ++next;
            ptr->array[ptr->size].x = first;
            ptr->array[ptr->size].y = strtold(next, NULL);
            ptr->array[ptr->size].mag = hypotl(first, ptr->array[ptr->size].y);
            ptr->array[ptr->size].deg = atan2l(ptr->array[ptr->size].y, first) * 180.0 / M_PI;
            if(ptr->array[ptr->size].deg < 0.0)
            {
                ptr->array[ptr->size].deg += 360.0;
            }
            ++(ptr->size);
        }
        else if(*next == '@')
        {
            ++next;
            long double numerator;
            long double angle = strtold(next, &next);

            if(*next == ',')
            {
                ++next;
                numerator = strtold(next, NULL);
                angle = atan2l(numerator, angle);

                ptr->array[ptr->size].x = first * cosl(angle);
                ptr->array[ptr->size].y = first * sinl(angle);
                ptr->array[ptr->size].deg = angle * 180/M_PI;
                if(ptr->array[ptr->size].deg < 0.0)
                {
                    ptr->array[ptr->size].deg += 360.0;
                }
                ptr->array[ptr->size].mag = first;

            }
            else
            {
                ptr->array[ptr->size].x = first * cosl(angle * M_PI / 180.0);
                ptr->array[ptr->size].y = first * sinl(angle * M_PI / 180.0);
                ptr->array[ptr->size].deg = angle;
                ptr->array[ptr->size].mag = first;
            }
            ++(ptr->size);
        }
        else
        {
            fprintf(stderr, "invalid input\n");
            exit(1);
        }
        ret = 1;
    }
    return ret;
}
