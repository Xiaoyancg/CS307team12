#pragma once

#define STRING(name) #name

template <typename T>
static void memoryPrinter(T *d, const char *name, size_t length = sizeof(T), int offset = 0)
{
    unsigned char *p = (unsigned char *)(d) + (offset);
    printf("\nMemory of [%s] of type [%s]\nat: [%p]", name, typeid(d).name(), p);
    printf(", length: [%zu], offset: [%d]\n[BEGIN]\n", length, offset);
    for (size_t i = 0; i < length; i++)
    {
        printf("%02x ", p[i]);
        if (i % 4 == 3)
            printf("| ");
        if (i % 16 == 15)
            printf("\n");
    }
    printf("[END]\n");
}