#ifndef CPCDOS_MANAGEDALLOC
#define CPCDOS_MANAGEDALLOC

#include "cpinti/types.h"

class ManagedAlloc
{
private:
    uinteger nb_alloc_do = 0;
    int managed_alloc_max = 1024;
    struct alloc_array
    {
        void *address;
    };
    alloc_array *Alloc_Array;

    const char *name_;

    uinteger NombreMalloc;

    uinteger NombreCalloc;

    uinteger NombreFree;

    uinteger NombreRealloc;

public:
    void ManagedAlloc_(size_t alloc_max, const char *filename);

    bool ManagedFree(void *ptr);

    int ManagedAlloc_clean();

    void *ManagedMalloc(size_t size__);

    void *ManagedCalloc(size_t size__, size_t sizeElem__);

    void *ManagedRealloc(void *ptr, size_t size__);

    void dump_memory(void);
};

#endif /* CPCDOS_MANAGEDALLOC */
