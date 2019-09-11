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
    int flag = 0;
    char buf[80];
    char* p1;
    char* p2;
    char* p3;
    long double value_1;
    long double value_2;
    long double value_3;

    fprintf(stdout, "%i:>> ", count++);
    fgets(buf, 80, stdin);

    //stop accepting vectors if the user enters "quit" or an empty line
    if((strcmp(buf,"\n")) != 0 && (strcmp(buf, "quit\n") != 0))
    {
        //Check the array size and reallocate it with more space if necessary
        if(ptr->capacity == ptr->size)
        {
            ptr->capacity <<= 1;
            ptr->array = (struct element*) realloc(ptr->array, ptr->capacity * sizeof(struct element));
        }
        if(strchr(buf, ',') != NULL)
        {
            flag += 1;
        }
        if(strchr(buf, '@') != NULL)
        {
            flag += 2;
        }

        p1 = strtok(buf, ",@");
        p2 = strtok(NULL, ",@");
        p3 = strtok(NULL, ",@");

        value_1 = strtold(p1, NULL);
        value_2 = strtold(p2, NULL);
        if(p3 != NULL)
        {
            value_3 = strtold(p3, NULL);
        }

        if(flag == 1)
        {
            ptr->array[ptr->size].x = value_1;
            ptr->array[ptr->size].y = value_2;
            ptr->array[ptr->size].mag = hypotl(ptr->array[ptr->size].x, ptr->array[ptr->size].y);
            ptr->array[ptr->size].deg = atan2l(ptr->array[ptr->size].y, ptr->array[ptr->size].x) * 180.0 / M_PI;
            if(ptr->array[ptr->size].deg < 0.0) //convert angles to 0/360 instead of +180/-180
            {
                ptr->array[ptr->size].deg += 360.0;
            }
        }
        else if(flag == 2)
        {
            ptr->array[ptr->size].mag = value_1;
            ptr->array[ptr->size].deg = value_2;
            ptr->array[ptr->size].x = value_1 * cosl(value_2 * M_PI / 180);
            ptr->array[ptr->size].y = value_1 * sinl(value_2 * M_PI / 180);
        }
        else if(flag == 3)
        {
            long double angle = atan2l(value_3, value_2);

            ptr->array[ptr->size].mag = value_1;
            ptr->array[ptr->size].deg = angle * 180/M_PI;
            if(ptr->array[ptr->size].deg < 0.0)
            {
                ptr->array[ptr->size].deg += 360.0;
            }
            ptr->array[ptr->size].x = value_1 * cosl(angle);
            ptr->array[ptr->size].y = value_1 * sinl(angle);
        }
        else
        {
            fprintf(stderr, "invalid input\n");
            exit(1);
        }

        ++(ptr->size);
        ret = 1;
    }
    return ret;
}
