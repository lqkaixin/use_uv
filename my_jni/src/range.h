#include "define.h"

#include <list>

typedef struct Range_s
{
	int64 pos, len;
}Range;

typedef struct Range_list_s
{
	std::list<Range> rgs;
}RangeList;


int rgs_add_rg(RangeList *rgs, const Range rg);
int rgs_rm_rg(RangeList *rgs, const Range rg);
int rgs_add_rgs(RangeList *rgs1, const RangeList *rgs2);
int rgs_rm_rgs(RangeList *rgs1, const RangeList *rgs2);
int rgs_intersection_rg(RangeList *rgs, const Range rg);
int rgs_intersection_rgs(RangeList *rgs1, const RangeList *rgs2);