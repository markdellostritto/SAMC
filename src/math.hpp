#ifndef MATH_HPP
#define MATH_HPP

namespace math{

        template <class T> inline T mod(T n, T z)noexcept{return (n%z+z)%z;}
        template <> inline int mod<int>(int n, int z)noexcept{return (n%z+z)%z;}
        template <> inline unsigned int mod<unsigned int>(unsigned int n, unsigned int z)noexcept{return (n%z+z)%z;}
        template <> inline float mod<float>(float n, float z)noexcept{return fmod(fmod(n,z)+z,z);}
        template <> inline double mod<double>(double n, double z)noexcept{return fmod(fmod(n,z)+z,z);}
        template <class T> inline T mod(T n, T lLim, T uLim)noexcept{return mod<T>(n-lLim,uLim-lLim)+lLim;}

}

#endif
