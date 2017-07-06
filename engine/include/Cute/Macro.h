//! API
#pragma once

//////////////////////////////////////////////////////////////////////////
// Macro
//////////////////////////////////////////////////////////////////////////
#define CUTE_PARAM_MAX				20

//
// CUTE_JOIN
//
// The following piece of macro magic joins the two
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in CUTE_DO_JOIN2 but does in CUTE_DO_JOIN.
//
#define CUTE_JOIN(X, Y)				CUTE_DO_JOIN(X, Y)
#define CUTE_DO_JOIN(X, Y)			CUTE_DO_JOIN2(X, Y)
#define CUTE_DO_JOIN2(X, Y)			X##Y

#define CUTE_EMPTY()
#define CUTE_COMMA()				,
#define CUTE_STRINGIZE(s)			PPI_STRINGIZE(s)
#define CUTE_CONCAT(a,b)			PPI_CONCAT(a, b)
#define CUTE_INC(N)					PPI_INC(N)
#define CUTE_DEC(N)					PPI_DEC(N)
#define CUTE_BOOL(N)				PPI_BOOL(N)

#define CUTE_IF(N, T, F)			PPI_IF(N, T, F)						//N==0:执行F,其他执行T
#define CUTE_DONE_IF(N, D)			PPI_DONE_IF(N, D)					//N!=0:执行D
#define CUTE_COMMA_IF(N)			PPI_COMMA_IF(N)						//N!=0时添加,
#define CUTE_COMMA_PARAM(N, P)		PPI_COMMA_PARAM(N, P)				//N!=0时添加,并在后边追加P##N

// N - repeat count, M - macro to execute M(N, P), P - extra parameter to be passed to M
// 1和2差别不大，可以嵌套用
#define CUTE_REPEAT(N, M)			PPI_REPEAT(N, M, CUTE_EMPTY)
#define CUTE_REPEAT_P(N, M, P)		PPI_REPEAT(N, M, P)

#define CUTE_REPEAT2(N, M)			PPI_REPEAT2(N, M, CUTE_EMPTY)
#define CUTE_REPEAT2_P(N, M, P)		PPI_REPEAT2(N, M, P)

#define CUTE_REPEAT_PARAMS(N, P)	PPI_REPEAT_PARAMS(N, P)

// function template
// typename T0, typename T1 ...
#define CUTE_LIST_TYPENAME(N)			CUTE_REPEAT2(N, CUTE_LIST_TYPENAME_DO)
#define CUTE_LIST_TYPENAME_DO(N, P)		PPI_COMMA_PARAM(N, typename T)

// T0, T1 ...
#define CUTE_LIST_TYPE(N)				CUTE_REPEAT2(N, CUTE_LIST_TYPE_DO)
#define CUTE_LIST_TYPE_DO(N, P)			PPI_COMMA_PARAM(N, T)

// T1, T2...
#define CUTE_LIST_TYPE_INC(N)			CUTE_REPEAT2(N, CUTE_LIST_TYPE_INC_DO)
#define CUTE_LIST_TYPE_INC_DO(N, P)		PPI_COMMA_PARAM(PPI_INC(N), T)

// t0, t1 ...
#define CUTE_LIST_ARGS(N)				CUTE_REPEAT2(N, CUTE_LIST_ARGS_DO)
#define CUTE_LIST_ARGS_DO(N, P)			PPI_COMMA_PARAM(N, t)

// T0 t0, T1 t1 ...
#define CUTE_LIST_DEFS(N)				CUTE_REPEAT2(N, CUTE_LIST_DEFS_DO)
#define CUTE_LIST_DEFS_DO(N, P)			PPI_COMMA_IF(N) T##N t##N

//////////////////////////////////////////////////////////////////////////
// PreProcessor Internal
//////////////////////////////////////////////////////////////////////////
#define PPI_EMPTY()
#define PPI_COMMA()					,
#define PPI_DONE(s)					s					// 执行s

#define PPI_STRINGIZE(s)			PPI_STRINGIZE_DO(s)
#define PPI_STRINGIZE_DO(s)			#s

#define PPI_CONCAT(a, b)			PPI_CONCAT_DO(a, b)
#define PPI_CONCAT_DO(a, b)			a##b

#define PPI_INC(N)					PPI_CONCAT(PPI_INC_, N)
#define PPI_DEC(N)					PPI_CONCAT(PPI_DEC_, N)
#define PPI_BOOL(N)					PPI_CONCAT(PPI_BOOL_,N)
#define PPI_REPEAT(N, M, P)			PPI_CONCAT(PPI_REPEAT_1_,N(M, P))
#define PPI_REPEAT2(N,M, P)			PPI_CONCAT(PPI_REPEAT_2_,N(M, P))
#define PPI_REPEAT_PARAMS(N,P)		PPI_REPEAT(N, PPI_COMMA_PARAM, P)	// (,)P1,P2,P3
//#define PPI_REPEAT(N, M, P)		PPI_REPEAT_1_ ## N(M, P)
//#define PPI_REPEAT2(N,M, P)		PPI_REPEAT_2_ ## N(M, P)

#define PPI_IF(N, T, F)				PPI_IF_DO1(PPI_BOOL(N), T, F)		// 将N转化成0或1
#define PPI_IF_DO1(B, T, F)			PPI_IF_DO2(B, T, F)					// 封装一层调用
#define PPI_IF_DO2(B, T, F)			PPI_IF_DO3(PPI_IF_ ## B (T, F))		// 拼接PPI_IF_X(T, F)的形式
#define PPI_IF_DO3(M)				M									// 调用PPI_IF_X(T, F)
#define PPI_IF_1(T, F)				T									// 执行T
#define PPI_IF_0(T, F)				F									// 执行F

#define PPI_DONE_IF(N, D)			PPI_IF(N, D, PPI_EMPTY)()			// N!=0时执行D
#define PPI_COMMA_IF(N)				PPI_DONE_IF(N, PPI_COMMA)			// N!=0时添加逗号','
#define PPI_COMMA_PARAM(N,P)		PPI_COMMA_IF(N) P##N				// N!=0时添加,并在后边追加P##N

//////////////////////////////////////////////////////////////////////////
// repeat1
//////////////////////////////////////////////////////////////////////////
#define PPI_REPEAT_1_0(M, P)
#define PPI_REPEAT_1_1(M, P)	M(0, P)
#define PPI_REPEAT_1_2(M, P)	PPI_REPEAT_1_1(M, P)  M(1, P)
#define PPI_REPEAT_1_3(M, P)	PPI_REPEAT_1_2(M, P)  M(2, P)
#define PPI_REPEAT_1_4(M, P)	PPI_REPEAT_1_3(M, P)  M(3, P)
#define PPI_REPEAT_1_5(M, P)	PPI_REPEAT_1_4(M, P)  M(4, P)
#define PPI_REPEAT_1_6(M, P)	PPI_REPEAT_1_5(M, P)  M(5, P)
#define PPI_REPEAT_1_7(M, P)	PPI_REPEAT_1_6(M, P)  M(6, P)
#define PPI_REPEAT_1_8(M, P)	PPI_REPEAT_1_7(M, P)  M(7, P)
#define PPI_REPEAT_1_9(M, P)	PPI_REPEAT_1_8(M, P)  M(8, P)
#define PPI_REPEAT_1_10(M, P)	PPI_REPEAT_1_9(M, P)  M(9, P)
#define PPI_REPEAT_1_11(M, P)	PPI_REPEAT_1_10(M, P) M(10, P)
#define PPI_REPEAT_1_12(M, P)	PPI_REPEAT_1_11(M, P) M(11, P)
#define PPI_REPEAT_1_13(M, P)	PPI_REPEAT_1_12(M, P) M(12, P)
#define PPI_REPEAT_1_14(M, P)	PPI_REPEAT_1_13(M, P) M(13, P)
#define PPI_REPEAT_1_15(M, P)	PPI_REPEAT_1_14(M, P) M(14, P)
#define PPI_REPEAT_1_16(M, P)	PPI_REPEAT_1_15(M, P) M(15, P)
#define PPI_REPEAT_1_17(M, P)	PPI_REPEAT_1_16(M, P) M(16, P)
#define PPI_REPEAT_1_18(M, P)	PPI_REPEAT_1_17(M, P) M(17, P)
#define PPI_REPEAT_1_19(M, P)	PPI_REPEAT_1_18(M, P) M(18, P)
#define PPI_REPEAT_1_20(M, P)	PPI_REPEAT_1_19(M, P) M(19, P)
#define PPI_REPEAT_1_21(M, P)	PPI_REPEAT_1_20(M, P) M(20, P)

//////////////////////////////////////////////////////////////////////////
// repeat2, 用于嵌套使用
//////////////////////////////////////////////////////////////////////////
#define PPI_REPEAT_2_0(M, P)
#define PPI_REPEAT_2_1(M, P)	M(0, P)
#define PPI_REPEAT_2_2(M, P)	PPI_REPEAT_2_1(M, P) M(1, P)
#define PPI_REPEAT_2_3(M, P)	PPI_REPEAT_2_2(M, P) M(2, P)
#define PPI_REPEAT_2_4(M, P)	PPI_REPEAT_2_3(M, P) M(3, P)
#define PPI_REPEAT_2_5(M, P)	PPI_REPEAT_2_4(M, P) M(4, P)
#define PPI_REPEAT_2_6(M, P)	PPI_REPEAT_2_5(M, P) M(5, P)
#define PPI_REPEAT_2_7(M, P)	PPI_REPEAT_2_6(M, P) M(6, P)
#define PPI_REPEAT_2_8(M, P)	PPI_REPEAT_2_7(M, P) M(7, P)
#define PPI_REPEAT_2_9(M, P)	PPI_REPEAT_2_8(M, P) M(8, P)
#define PPI_REPEAT_2_10(M, P)	PPI_REPEAT_2_9(M, P) M(9, P)
#define PPI_REPEAT_2_11(M, P)	PPI_REPEAT_2_10(M, P) M(10, P)
#define PPI_REPEAT_2_12(M, P)	PPI_REPEAT_2_11(M, P) M(11, P)
#define PPI_REPEAT_2_13(M, P)	PPI_REPEAT_2_12(M, P) M(12, P)
#define PPI_REPEAT_2_14(M, P)	PPI_REPEAT_2_13(M, P) M(13, P)
#define PPI_REPEAT_2_15(M, P)	PPI_REPEAT_2_14(M, P) M(14, P)
#define PPI_REPEAT_2_16(M, P)	PPI_REPEAT_2_15(M, P) M(15, P)
#define PPI_REPEAT_2_17(M, P)	PPI_REPEAT_2_16(M, P) M(16, P)
#define PPI_REPEAT_2_18(M, P)	PPI_REPEAT_2_17(M, P) M(17, P)
#define PPI_REPEAT_2_19(M, P)	PPI_REPEAT_2_18(M, P) M(18, P)
#define PPI_REPEAT_2_20(M, P)	PPI_REPEAT_2_19(M, P) M(19, P)
#define PPI_REPEAT_2_21(M, P)	PPI_REPEAT_2_20(M, P) M(20, P)

//////////////////////////////////////////////////////////////////////////
//dec
//////////////////////////////////////////////////////////////////////////
#define PPI_DEC_1			0
#define PPI_DEC_2			1
#define PPI_DEC_3			2
#define PPI_DEC_4			3
#define PPI_DEC_5			4
#define PPI_DEC_6			5
#define PPI_DEC_7			6
#define PPI_DEC_8			7
#define PPI_DEC_9			8
#define PPI_DEC_10			9
#define PPI_DEC_11			10
#define PPI_DEC_12			11
#define PPI_DEC_13			12
#define PPI_DEC_14			13
#define PPI_DEC_15			14
#define PPI_DEC_16			15
#define PPI_DEC_17			16
#define PPI_DEC_18			17
#define PPI_DEC_19			18
#define PPI_DEC_20			19
#define PPI_DEC_21			20

//////////////////////////////////////////////////////////////////////////
// inc
//////////////////////////////////////////////////////////////////////////
#define PPI_INC_0			1
#define PPI_INC_1			2
#define PPI_INC_2			3
#define PPI_INC_3			4
#define PPI_INC_4			5
#define PPI_INC_5			6
#define PPI_INC_6			7
#define PPI_INC_7			8
#define PPI_INC_8			9
#define PPI_INC_9			10
#define PPI_INC_10			11
#define PPI_INC_11			12
#define PPI_INC_12			13
#define PPI_INC_13			14
#define PPI_INC_14			15
#define PPI_INC_15			16
#define PPI_INC_16			17
#define PPI_INC_17			18
#define PPI_INC_18			19
#define PPI_INC_19			20
#define PPI_INC_20			21

//////////////////////////////////////////////////////////////////////////
// bool
//////////////////////////////////////////////////////////////////////////
#define PPI_BOOL_0			0
#define PPI_BOOL_1			1
#define PPI_BOOL_2			1
#define PPI_BOOL_3			1
#define PPI_BOOL_4			1
#define PPI_BOOL_5			1
#define PPI_BOOL_6			1
#define PPI_BOOL_7			1
#define PPI_BOOL_8			1
#define PPI_BOOL_9			1
#define PPI_BOOL_10			1
#define PPI_BOOL_11			1
#define PPI_BOOL_12			1
#define PPI_BOOL_13			1
#define PPI_BOOL_14			1
#define PPI_BOOL_15			1
#define PPI_BOOL_16			1
#define PPI_BOOL_17			1
#define PPI_BOOL_18			1
#define PPI_BOOL_19			1
#define PPI_BOOL_20			1
#define PPI_BOOL_21			1
