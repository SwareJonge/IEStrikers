#ifndef NW4R_UT_COLOR_H
#define NW4R_UT_COLOR_H

#include <revolution/GX/GXTypes.h>
/*
#define r channels[0]
#define g channels[1]
#define b channels[2]
#define a channels[3]
/**/
namespace nw4r
{
    namespace ut
    {
        struct Color
        {
            GXColor mChannels;

            inline u32 &ToU32ref()
            {
                return *reinterpret_cast<u32 *>(this);
            }

            inline const u32 &ToU32ref() const
            {
                return *reinterpret_cast<const u32 *>(this);
            }

            inline Color &operator=(u32 value)
            {
                ToU32ref() = value;
                return *this;
            }

            inline Color()
            {
                *this = 0xFFFFFFFF;
            }

            inline Color(const Color &other)
            {
                mChannels.r = other.mChannels.r;
                mChannels.g = other.mChannels.g;
                mChannels.b = other.mChannels.b;
                mChannels.a = other.mChannels.a;
            }

            inline Color &operator=(const Color &other)
            {
                mChannels = other.mChannels;

                return *this;
            }

            inline Color(u32 rgba)
            {
                *this = rgba;
            }

            inline Color(int red, int green, int blue, int alpha)
            {
                mChannels.r = red;
                mChannels.g = green;
                mChannels.b = blue;
                mChannels.a = alpha;
            }

            inline operator u32() const
            {
                return ToU32ref();
            }

            inline operator GXColor() const
            {
                return mChannels;
            }

            inline ~Color() {}
        };
    }
}

#endif