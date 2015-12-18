#pragma once
// 一些常用宏定义
//////////////////////////////////////////////////////////////////////////
// 常用宏
//////////////////////////////////////////////////////////////////////////
#define CU_NO_USE(x)				(void)(x)
#define CU_NONCOPYABLE(CLASS)		CLASS(CLASS const&); CLASS& operator=(CLASS const&)

#define CU_SAFE_DELETE(ptr)			if(ptr){ delete ptr; ptr = NULL; }
#define CU_SAFE_RELEASE(ptr)		if(ptr){ ptr->release(); ptr = NULL; }
#define CU_HAS_MASK(data, flag)		((data & flag) == flag)
#define CU_SET_MASK(data, flag)		data |= flag
#define CU_CLEAR_MASK(data, flag)	data &= ~flag
#define CU_FOURCC(a,b,c,d)			( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) | ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )
#define CU_ARRAY_SIZE(a)			(sizeof(a)/sizeof(a[0]))

#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
