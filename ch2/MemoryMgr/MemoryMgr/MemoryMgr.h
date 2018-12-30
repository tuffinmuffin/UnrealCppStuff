#pragma once

#ifdef _WIN32
#ifdef MEMORYMGR_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#elif __APPLE__
#define EXPORT __attribute__((visibility("default")))
#endif


struct Block
{
	Block* next;
};

struct Heap
{
	Heap* next;

	//Return the first block
	Block* block()
	{
		return reinterpret_cast<Block*>(this + 1);
	}
};
	


class EXPORT CMemoryMgr
{
public:
	CMemoryMgr(unsigned int dataSize, unsigned int heapSize, unsigned int memoryAlightment);
	~CMemoryMgr();

	void* Allocate();

	void Deallocate(void* pointerToMemory);
	void DeallocateAll();

private:
	static const unsigned char ALLOCATION_PATTERN = 0xBEEF;
	static const	 unsigned char ALIGHTMENT_PATTERN = 0xBADD;
	static const unsigned char FREE_MEMORY_PATTERN = 0xF00D;

	Block* GetNextBlock(Block* block);

	void OverWriteHeap(Heap* heapPointer);
	void OverWriteBlock(Block* blockPointer);
	void OverWriteAllocatedBlock(Block* blockPointer);

	Heap* m_heapList = nullptr;
	Block* m_freeBlockList = nullptr;

	unsigned int m_dataSize;
	unsigned int m_heapSize;
	unsigned int m_memoryAlignment;
	unsigned int m_blockSize;
	unsigned int m_blocksPerHeap;

	unsigned int m_numOfHeaps;
	unsigned int m_numOfBlocks;
	unsigned int m_numOfBlocksFree;
};

