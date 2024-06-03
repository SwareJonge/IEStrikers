#pragma once

/*
    Mobiclip Fast Audio Codec
*/

#include <string.h>

#include <revolution/types.h>

#ifdef __cplusplus

class Codec {
public:
    ~Codec() {}

    void Init() {
        PreprocessingOffset = 0;
        PreprocessingOffset2 = 0;
        PreprocessingPreemphasis = 0;
        PostprocessingOffset = 0;

        memset(ShortTermAnalysis, 0, sizeof(ShortTermAnalysis));
        memset(ShortTermSynthesis, 0, sizeof(ShortTermSynthesis));
        memset(ShortTermSynthesisBackup, 0, sizeof(ShortTermSynthesisBackup));
        memset(Previous, 0, sizeof(Previous));
    }

    void UnpackFast(const s8 *in, s16 *out);

private:
    short PreprocessingOffset;
    int PreprocessingOffset2;
    int PreprocessingPreemphasis;
    short PostprocessingOffset;
    short ShortTermAnalysis[8];
    short ShortTermSynthesis[9];
    short ShortTermSynthesisBackup[9];
    short Previous[192];
};

#endif