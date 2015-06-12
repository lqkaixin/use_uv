#include "range.h"


int rgs_add_rg(RangeList *rgs, const Range rg1)
{
	Range rg = rg1;
	std::list<Range>::iterator ite = rgs->rgs.begin();
	while (ite != rgs->rgs.end())
	{
		if (rg.pos > (*ite).pos+(*ite).len)
			ite++;
		else if (rg.pos+rg.len < (*ite).pos)
		{
			rgs->rgs.insert(ite, rg);
			return 0;
		}
		else if (rg.pos <= (*ite).pos)
		{
			if (rg.pos+rg.len <= (*ite).pos+(*ite).len)
			{
				(*ite).len += (*ite).pos-rg.pos;
				(*ite).pos = rg.pos;
				return 0;
			}
			else
			{
				ite = rgs->rgs.erase(ite);
				continue;
			}
			
		}
		else
		{
			if (rg.pos+rg.len < (*ite).pos+(*ite).len)
			{
				return 0;
			}
			else 
			{
				rg.len = rg.pos+rg.len-(*ite).pos;
				rg.pos = (*ite).pos;
				ite = rgs->rgs.erase(ite);
			}

		}
	}
	rgs->rgs.push_back(rg);
	return 0;
}
int rgs_rm_rg(RangeList *rgs, const Range rg1)
{
	Range rg = rg1;
	std::list<Range>::iterator ite = rgs->rgs.begin();
	while (ite != rgs->rgs.end())
	{
		if (rg.pos > (*ite).pos+(*ite).len)
			ite++;
		else if (rg.pos+rg.len < (*ite).pos)
		{
			return 0;
		}
		else if (rg.pos <= (*ite).pos)
		{
			if (rg.pos+rg.len <= (*ite).pos+(*ite).len)
			{
				(*ite).len = (*ite).pos+(*ite).len - (rg.pos+rg.len);
				(*ite).pos = rg.pos+rg.len;
				return 0;
			}
			else
			{
				ite = rgs->rgs.erase(ite);
				continue;
			}
			
		}
		else
		{
			if (rg.pos+rg.len < (*ite).pos+(*ite).len)
			{
				Range rg2;
				rg2.pos = (*ite).pos;
				rg2.len = rg.pos - (*ite).pos;

				(*ite).len = (*ite).pos+(*ite).len - (rg.pos+rg.len);
				(*ite).pos = rg.pos+rg.len;

				rgs->rgs.insert(ite, rg2);

				return 0;
			}
			else 
			{
				(*ite).len = rg.pos-(*ite).pos;
				ite++;
			}

		}
	}
	return 0;
}
int rgs_add_rgs(RangeList *rgs1, const RangeList *rgs2)
{
	RangeList *rgs = rgs1;
	std::list<Range>::iterator ite = rgs->rgs.begin();
	std::list<Range>::const_iterator ite2 = rgs2->rgs.begin();

	while(ite2 != rgs2->rgs.end())
	{
		Range rg = *ite2;
		
		while (ite != rgs->rgs.end())
		{
			if (rg.pos > (*ite).pos+(*ite).len)
				ite++;
			else if (rg.pos+rg.len < (*ite).pos)
			{
				rgs->rgs.insert(ite, rg);
				break;
			}
			else if (rg.pos <= (*ite).pos)
			{
				if (rg.pos+rg.len <= (*ite).pos+(*ite).len)
				{
					(*ite).len += (*ite).pos-rg.pos;
					(*ite).pos = rg.pos;
					break;
				}
				else
				{
					ite = rgs->rgs.erase(ite);
					continue;
				}
				
			}
			else
			{
				if (rg.pos+rg.len < (*ite).pos+(*ite).len)
				{
					break;
				}
				else 
				{
					rg.len = rg.pos+rg.len-(*ite).pos;
					rg.pos = (*ite).pos;
					ite = rgs->rgs.erase(ite);
				}

			}
		}
		if (ite == rgs->rgs.end()) rgs->rgs.push_back(rg);

		ite2++;
	}
	return 0;
}
int rgs_rm_rgs(RangeList *rgs1, const RangeList *rgs2)
{
	RangeList *rgs = rgs1;
	std::list<Range>::iterator ite = rgs->rgs.begin();
	std::list<Range>::const_iterator ite2 = rgs2->rgs.begin();

	while (ite2 != rgs2->rgs.end())
	{
		Range rg = *ite2;
		while (ite != rgs->rgs.end())
		{
			if (rg.pos > (*ite).pos+(*ite).len)
				ite++;
			else if (rg.pos+rg.len < (*ite).pos)
			{
				break;
			}
			else if (rg.pos <= (*ite).pos)
			{
				if (rg.pos+rg.len <= (*ite).pos+(*ite).len)
				{
					(*ite).len = (*ite).pos+(*ite).len - (rg.pos+rg.len);
					(*ite).pos = rg.pos+rg.len;
					break;
				}
				else
				{
					ite = rgs->rgs.erase(ite);
					continue;
				}
				
			}
			else
			{
				if (rg.pos+rg.len < (*ite).pos+(*ite).len)
				{
					Range rg2;
					rg2.pos = (*ite).pos;
					rg2.len = rg.pos - (*ite).pos;

					(*ite).len = (*ite).pos+(*ite).len - (rg.pos+rg.len);
					(*ite).pos = rg.pos+rg.len;

					rgs->rgs.insert(ite, rg2);

					break;
				}
				else 
				{
					(*ite).len = rg.pos-(*ite).pos;
					ite++;
				}

			}
		}
		ite2++;
	}
	return 0;
}
int rgs_intersection_rg(RangeList *rgs, const Range rg)
{
	;
}
int rgs_intersection_rgs(RangeList *rgs1, const RangeList *rgs2)
{
	;
}