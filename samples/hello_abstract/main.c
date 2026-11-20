#include <ps5/kernel.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t (*Fn3)(uint64_t, uint64_t, uint64_t);

static const char* NAMES[] = {
    "_ZN15AbstractStorage12LocalContent8SeekTimeElPl",
    "_ZN15AbstractStorage12LocalStorage14GetStorageSizeEPlS1_S1_",
    "_ZNK15AbstractStorage11LocalFolder10GetServiceEv",
    "_ZNK15AbstractStorage12LocalContent10GetServiceEv",
    "_ZNK15AbstractStorage12LocalContent13GetCapabilityEv",
    "_ZNK15AbstractStorage12LocalService14GetServiceTypeEv",
    "_ZNK15AbstractStorage12LocalStorage10GetServiceEv",
    "_ZNK15AbstractStorage12LocalStorage7GetStatEPNS_4StatE",
};

static const uint64_t EXPECTED[] = {
    0x809e0005u, 0x809e0002u, 0u, 0u, 0x37c5au, 0u, 0u, 0x809e0002u,
};

int main(void) {
    printf("FW|0x%x\n", kernel_get_fw_version());
    void* lib = dlopen("libSceAbstractLocal.sprx", RTLD_LAZY);
    if (!lib) { printf("NOPKG|%s\n", dlerror()); return 1; }
    unsigned mismatches = 0;
    for (unsigned i = 0; i < sizeof(NAMES)/sizeof(NAMES[0]); ++i) {
        Fn3 f = (Fn3)dlsym(lib, NAMES[i]);
        if (!f) { printf("MISS|%u\n", i); ++mismatches; continue; }
        uint64_t v = f(0, 0, 0) & 0xFFFFFFFFu;
        if (v != EXPECTED[i]) ++mismatches;
        printf("CASE|%u|0x%llx|0x%llx\n", i, (unsigned long long)v, (unsigned long long)EXPECTED[i]);
    }
    printf("RESULT|mismatches=%u\n", mismatches);
    return mismatches != 0;
}
