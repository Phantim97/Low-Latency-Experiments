#include <memory>

#include "ScopedTimer.h"
#include "goal.h"

//Prompt:
/**
 * \brief
 * You have a set of parquet tiles of size 3x1 and 2x1 length units.
 * Parquet tiles are always laid out in strips, for instance 3223 is a strip of length 10.
 * When two strips are adjacent, no two tiles can share an inner edge. 
 * For instance, strips 3223 and 3322 both start with a 3 so they cannot be neighbors.
 * But strips 3223 and 22222 of length 10 can be neighbors because they don't share inner edges.
 * Write a program in C/C++, that does the following:
 * Find the total number of floor designs which have 11 strips of length 30.  Provide the answer with the solution.
 * \return
 */

void test_bench()
{
	ScopedTimer<std::chrono::milliseconds>();
	bench_main();
}

int main()
{
	test_bench();
	return 0;
}