#include <iostream>

__declspec(dllexport) void Hello()
{
    std::cout << "Hello From a DLL" << std::endl;
    std::cout << "New DLL" << std::endl;
}
