#include "MemoryMgr.h"
#include <iostream>
#include <string>
#include <vector>

int main()
{
	CMemoryMgr mgr(sizeof(unsigned), 1024, 4);

	unsigned* i = reinterpret_cast<unsigned*> (mgr.Allocate());

	*i = 0u;
	std::getchar();
	mgr.Deallocate(i);
	std::vector<unsigned*> j;

	for (int i = 0; i < 104857600 / 10; i++)
	{
		j.push_back(reinterpret_cast<unsigned*> (mgr.Allocate()));
	}
	std::getchar();

	for (auto* i : j)
		mgr.Deallocate(i);


	return 0;
}
