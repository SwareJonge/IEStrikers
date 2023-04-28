#include <revolution/OS.h>
#include <revolution/SC.h>
#include <string.h>
#include <stdio.h>
#define XOR_KEY 0x73B5DBFA

static SCArea ProductAreaAndStringTbl[] = {
    {SC_AREA_JPN, "JPN"},
    {SC_AREA_USA, "USA"},
    {SC_AREA_EUR, "EUR"},
    {SC_AREA_AUS, "AUS"},
    {SC_AREA_BRA, "BRA"},
    {SC_AREA_TWN, "TWN"},
    {SC_AREA_TWN, "ROC"},
    {SC_AREA_KOR, "KOR"},
    {SC_AREA_HKG, "HKG"},
    {SC_AREA_ASI, "ASI"},
    {SC_AREA_LTN, "LTN"},
    {SC_AREA_SAF, "SAF"},
    {SC_AREA_CHN, "CHN"},
    {-1, ""}
};

static SCRegion ProductGameRegionAndStringTbl[] = {
    {SC_REGION_JP, "JP"},
    {SC_REGION_US, "US"},
    {SC_REGION_EU, "EU"},
    {SC_REGION_KR, "KR"},
    {SC_REGION_CN, "CN"},
    {-1, ""}
};

BOOL __SCF1(const char* type, char* buf, u32 sz) {
    u32 ptext; // retyped to u32, originally u8
    BOOL found = FALSE;
    u32 i;
    const u8* settings = (const u8*)OSPhysicalToCached(OS_PHYS_SC_PRDINFO);
    u32 key = XOR_KEY;
    u32 typeOfs = 0;
    u32 bufOfs = 0;

    for (i = 0; i < SC_PRDINFO_SIZE; i++, key = key >> 31 | key << 1) {
        ptext = settings[i];

        if (ptext != 0x00) {
            ptext = (u8)(ptext ^ key);
            if (type[typeOfs] == 0x00 && ptext == '=') {
                found = TRUE;
                break;
            }

            if (((ptext ^ type[typeOfs]) & 0xDF) == 0) {
                typeOfs++;
            } else {
                typeOfs = 0;
            }
        }
    }

    if (found) {
        for (i++; i < SC_PRDINFO_SIZE && bufOfs < sz; i++) {
            key = key >> 31 | key << 1;
            ptext = settings[i];

            if (ptext != 0x00) {
                ptext = (u8)(ptext ^ key);
                if (ptext == '\r' || ptext == '\n') {
                    ptext = 0x00;
                }
            }

            buf[bufOfs++] = ptext;

            if (ptext == 0x00) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL SCGetProductAreaString(char* buf, u32 sz) {
    return __SCF1("AREA", buf, sz);
}

s8 SCGetProductArea(void) {
    s8 area;
    char name[4];
    SCArea* iter = ProductAreaAndStringTbl;

    if (SCGetProductAreaString(name, sizeof(name))) {
        for (; (area = iter->area) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return area;
            }
        }
    }

    return -1;
}

BOOL SCGetProductCodeString(char* buf, u32 sz) {
    return __SCF1("CODE", buf, sz);
}

char *SCGetProductCode() {
    static char buf[6];

    if (!SCGetProductCodeString(buf, sizeof(buf))) // could be inline
        return 0;
    return buf;
}

BOOL SCGetProductSNString(char* buf, u32 sz) {
    return __SCF1("SERNO", buf, sz);
}

s8 SCGetProductSN(u32 ret) { // not entirely sure what type the parameter is
    u8 code;
    char sn[11];
    if (SCGetProductSNString(sn, sizeof(sn)) && sscanf(sn, "%u", ret) == 1) {
        code = 1;
    }
    else {
        code = 0;
    }
    return code;
}

BOOL SCGetGameCodeString(char* buf, u32 sz) {
    return __SCF1("GAME", buf, sz);
}

s8 SCGetProductGameRegion(void) {
    s8 region;
    char name[3];
    SCRegion* iter = ProductGameRegionAndStringTbl;

    if (SCGetGameCodeString(name, sizeof(name))) {
        for (; (region = iter->region) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return region;
            }
        }
    }

    return -1;
}