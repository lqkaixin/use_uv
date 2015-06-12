
#include "../range.h"

#include "log.h"

#define TLOG_DEBUG(fmt, ...) LOG_DEBUG("[%s] " fmt, title, __VA_ARGS__)

const char *title;


#define INIT \
RangeList rgs1,rgs2; \
Range rg; \
std::list<Range>::iterator ite, ite1;


#define PRE_RGS1(_pos,_end) \
rg.pos = _pos, rg.len = _end-_pos; \
rgs_add_rg(&rgs1, rg);

#define PRE_RGS2(_pos,_end) \
rg.pos = _pos, rg.len = _end-_pos; \
rgs_add_rg(&rgs2, rg);


#define OUTPUT \
TLOG_DEBUG("size: %zu", rgs1.rgs.size()); \
ite1 = rgs1.rgs.begin(); \
while (ite1 != rgs1.rgs.end()) \
{ \
	TLOG_DEBUG("rg: [%lld, %lld)", (*ite1).pos, (*ite1).pos+(*ite1).len); \
	ite1++; \
}

#define ASSERT(exp) if ((exp) == 0) { TLOG_DEBUG("error: %s", "sss"); OUTPUT}


#define ASSERT_SIZE(n) ASSERT(rgs1.rgs.size() == n) \
ite = rgs1.rgs.begin();

#define ASSERT_RG(_pos, _end) ASSERT(ite!=rgs1.rgs.end() && (*ite).pos == _pos && (*ite).pos+(*ite).len == _end) \
ite++;

static void test1_1()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(0,5);
	PRE_RGS1(8,10);
	PRE_RGS1(25,35);
	PRE_RGS1(45,60);


	ASSERT_SIZE(4);

	ASSERT_RG(0,5);
	ASSERT_RG(8,20);
	ASSERT_RG(25,40);
	ASSERT_RG(45,60);
}

static void test1_2()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(5,25);
	PRE_RGS1(28,50);


	ASSERT_SIZE(2);

	ASSERT_RG(5,25);
	ASSERT_RG(28,60);
}

static void test1_3()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(5,35);


	ASSERT_SIZE(2);

	ASSERT_RG(5,40);
	ASSERT_RG(50,60);
}

static void test1_4()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(5,40);


	ASSERT_SIZE(2);

	ASSERT_RG(5,40);
	ASSERT_RG(50,60);
}

static void test1_5()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(5,65);


	ASSERT_SIZE(1);

	ASSERT_RG(5,65);
}

static void test2_1()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(10,15);
	PRE_RGS1(30,40);
	PRE_RGS1(50,65);

	ASSERT_SIZE(3);

	ASSERT_RG(10,20);
	ASSERT_RG(30,40);
	ASSERT_RG(50,65);
}

static void test2_2()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(10,30);

	ASSERT_SIZE(2);

	ASSERT_RG(10,40);
	ASSERT_RG(50,60);
}

static void test2_3()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(10,35);

	ASSERT_SIZE(2);

	ASSERT_RG(10,40);
	ASSERT_RG(50,60);
}

static void test2_4()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(10,40);

	ASSERT_SIZE(2);

	ASSERT_RG(10,40);
	ASSERT_RG(50,60);
}

static void test2_5()
{
	TLOG_DEBUG("test1%s", "ss");

	INIT

	PRE_RGS1(10,20);
	PRE_RGS1(30,40);
	PRE_RGS1(50,60);

	PRE_RGS1(10,45);

	ASSERT_SIZE(2);

	ASSERT_RG(10,45);
	ASSERT_RG(50,60);
}




#define TEST_RGS_ADD_RG \
title = "test_rgs_add_rg"; \
test1_1(); \
test1_2(); \
test1_3(); \
test1_4(); \
test1_5(); \
test2_1(); \
test2_2(); \
test2_3(); \
test2_4(); \
test2_5(); \



void test_rg()
{
	TEST_RGS_ADD_RG
}