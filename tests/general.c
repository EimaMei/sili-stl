#define SI_IMPLEMENTATION 1
#include <sili.h>

typedef struct randomStruct {
	usize one;
	char two;
	f32 three;
} randomStruct;


#define TEST_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) == (arg2), format " | " format, arg1, arg2)
#define TEST_N_EQ(arg1, arg2, format) \
	SI_ASSERT_FMT((arg1) != (arg2), format " | " format, arg1, arg2)


#define TEST_EQ_U64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%lu")
#define TEST_EQ_H64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%#lx")
#define TEST_EQ_I64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%li")
#define TEST_EQ_F64(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%f")
#define TEST_EQ_CHAR(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%c")
#define TEST_EQ_PTR(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%p")
#define TEST_EQ_USIZE(arg1, arg2) \
	TEST_EQ(arg1, arg2, "%zu")



#define TEST_N_EQ_U64(arg1, arg2) \
	TEST_N_EQ(arg1, arg2, "%lu")



int main(void) {
	{
		TEST_EQ_U64(SI_KILO(1), 1024);
		TEST_EQ_U64(SI_MEGA(1), 1024 * 1024);
		TEST_EQ_U64(SI_GIGA(1), 1024 * 1024 * 1024);
		TEST_EQ_U64(SI_TERA(1), 1024 * 1024 * 1024 * 1024u);

		TEST_EQ_U64(SI_BIT(63), 0x8000000000000000);
		TEST_EQ_U64(nil, (void*)0);

		isize m = si_transmute(isize, USIZE_MAX, usize);
		TEST_EQ_I64(m, (isize)-1);

#if SI_ENDIAN_IS_LITTLE
		u32 value = 0x44434241;
#else
		u32 value = 0x41424344;
#endif

		cstring str = "ABCD";
		TEST_EQ_U64(SI_TO_U32(str), value);

		TEST_EQ_U64(si_offsetof(randomStruct, three), 12);
		TEST_EQ_U64(si_alignof(randomStruct), 8);

		char* buf1 = "QWERTY";
		char* buf2 = "AZERTY";
		si_swap(buf1, buf2);
		TEST_EQ_U64(strcmp(buf2, "QWERTY"), 0);
		TEST_EQ_U64(strcmp(buf1, "AZERTY"), 0);

		i16 x = 0;
		for_range (i, INT16_MIN, 0) {
			x -= 1;
		}
		TEST_EQ_I64(x, INT16_MIN);

		u64 src = 0x00FF00FF00FF00FF;
		u32 dst;
		memcpy_s(&dst, sizeof(dst), &src, sizeof(src));
		TEST_EQ_H64(dst, 0x00FF00FF);

		TEST_EQ_H64(0x44434241, si_swap32le(value));
		TEST_EQ_H64(0xFF00FF00FF00FF00, si_swap64(0x00FF00FF00FF00FF));
		TEST_EQ_H64(0xFF, si_swap16(0xFF00));

		u16 y[] = {0, UINT16_MAX};
		si_ptrMoveRight(&y[1], 2, 2);
		TEST_EQ_H64(y[0], 0xFFFF);

		y[0] = 0x8080;
		y[1] = 0;
		si_ptrMoveLeft(&y[0], 2, 2);
		TEST_EQ_H64(y[1], 0x8080);
	}
	si_print("Test 1 has been completed.\n");

	{
		usize ceil = si_alignCeilEx(12, 8);
		TEST_EQ_U64(ceil, 16);

		siAllocator alloc;
		rawptr ptr;
		usize avail;
		siAllocatorData inData;

		{
			alloc = si_allocatorHeap();
			TEST_EQ_PTR(alloc.proc, si_allocator_heap_proc);
			TEST_EQ_PTR(alloc.userData, nil);

			ptr = si_alloc(alloc, SI_KILO(1));
			si_free(alloc, ptr);

			avail = si_allocatorGetAvailable(alloc);
			TEST_EQ_USIZE(avail, USIZE_MAX);
		}

		{
			alloc = si_allocatorMakeArena(SI_MEGA(1), &inData);
			TEST_EQ_PTR(alloc.proc, si_allocator_stack_proc);
			TEST_EQ_PTR(alloc.userData, &inData);
			TEST_EQ_USIZE(inData.offset, 0);
			TEST_EQ_USIZE(inData.capacity, SI_MEGA(1));

			ptr = si_alloc(alloc, SI_KILO(1));
			TEST_EQ_USIZE(inData.offset, SI_KILO(1));
			TEST_EQ_PTR(ptr, inData.ptr);

			avail = si_allocatorGetAvailable(alloc);
			TEST_EQ_USIZE(avail, SI_MEGA(1) - SI_KILO(1));

			si_allocatorReset(alloc);
			TEST_EQ_USIZE(inData.offset, 0);

			si_freeAll(alloc);
			TEST_EQ_PTR(inData.ptr, nil);
			TEST_EQ_USIZE(inData.offset, 0);
			TEST_EQ_USIZE(inData.capacity, 0);
		}
#if 0
		char x[128];
		siAllocator tmp = si_allocatorMakeTmp(x, countof(x));
		TEST_EQ_H64(tmp.ptr, (siByte*)x);
		TEST_EQ_U64(tmp.capacity, countof(x));

		si_malloc(&alloc, si_alignCeil(234));
		TEST_EQ_U64(si_allocatorAvailable(alloc), alloc.capacity - si_alignCeil(234));

		si_allocatorResetFrom(&alloc, 444);
		TEST_EQ_U64(alloc.offset, 444);
		si_allocatorFree(&alloc);
		TEST_EQ_H64(alloc.ptr, 0);
#endif
		{
			siAllocator stack = si_allocatorMakeStack(32);
			char* x = si_alloc(stack, 1);
			*x = 'Q';
		}

		{
			usize* ptr1 = si_sallocItem(usize);
			*ptr1 = USIZE_MAX;
			TEST_EQ_H64(*ptr1, USIZE_MAX);

			alloc = si_allocatorMakeArena(SI_KILO(1), &inData);
			si_allocItem(alloc, randomStruct);
			si_allocArray(alloc, randomStruct, 3);

			TEST_EQ_U64(inData.offset, 4 * sizeof(randomStruct));

			si_freeAll(alloc);
		}
	}
	si_print("Test 2 has been completed.\n");

	{
		siAny any = SI_ANY(i32, 23);
		TEST_EQ_I64(any.typeSize, sizeof((i32)23));

		siPoint p1 = SI_POINT(50, 50),
				p2 = (siPoint){28, 28};
		TEST_EQ_U64(si_pointCmp(p1, p2), 0);

		siColor c1 = SI_RGBA(128, 128, 128, 255),
				c2 = SI_RGB(255, 0, 0),
				c3 = SI_HEX(0x808080);
		TEST_EQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c3));
		TEST_N_EQ_U64(SI_TO_U32(&c1), SI_TO_U32(&c2));

		siArea area = SI_AREA(2, 3);
		siRect r1 = SI_RECT(0, 1, 2, 3),
			   r2 = SI_RECT_A(4, 4, area),
			   r3 = SI_RECT_P(p1, 2, 3),
			   r4 = SI_RECT_PA(SI_POINT(0, 1), area);
		SI_UNUSED(r1), SI_UNUSED(r2), SI_UNUSED(r3), SI_UNUSED(r4);

		siVec2 v2 = SI_VEC2(2, 2);
		si_vec2Sub(&v2, SI_VEC2(-2, -2));
		TEST_EQ_F64(v2.x, 4);
		TEST_EQ_F64(v2.y, 4);
		si_vec2Add(&v2, SI_VEC2(-2, -2));
		TEST_EQ_F64(v2.x, 2);
		TEST_EQ_F64(v2.y, 2);
	}
	si_print("Test 4 has been completed.\n");

	{
		siOption(u64) opt = SI_OPT(u64, 19920216ULL);
		TEST_EQ_U64(opt.hasValue, 1);
		TEST_EQ_U64(opt.data.value, 19920216ULL);

		siError tmp = {0};
		tmp.code = 40;
		opt = SI_OPT_ERR(u64, tmp);
		TEST_EQ_I64(opt.data.error.code, 40);

		u64 res = si_optionalGetOrDefault(opt, UINT64_MAX);
		TEST_EQ_U64(res, UINT64_MAX);

		#if SI_STANDARD_CHECK_MIN(C, C11)
			opt = SI_OPT(u64, 19920216ULL);
			TEST_EQ_U64(opt.hasValue, 1);
			TEST_EQ_U64(opt.value, 19920216ULL);

			tmp.code = 40;
			opt = SI_OPT_ERR(u64, tmp);
			TEST_EQ_I64(opt.error.code, 40);

		#endif
	}
	si_print("Test 5 has been completed.\n");


	si_printf("%CTest '" __FILE__ "' has been completed!%C\n", si_printColor3bitEx(siPrintColorAnsi_Yellow, true, false));
}
