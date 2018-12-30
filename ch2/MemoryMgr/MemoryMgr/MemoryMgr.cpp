#include "MemoryMgr.h"

#include <algorithm> 

// This is an example of an exported variable
//EXPORT int nMemoryMgr=0;

// This is an example of an exported function.
//EXPORT int fnMemoryMgr(void)
//{
//}


// This is the constructor of a class that has been exported.
// see MemoryMgr.h for the class definition
CMemoryMgr::CMemoryMgr(unsigned int dataSize,
	unsigned int heapSize,
	unsigned int memoryAlignmentSize) :
	m_dataSize(dataSize), m_heapSize(heapSize)
{
	DeallocateAll();

	auto maxHeaderSize = std::max((unsigned int)sizeof(Block), m_dataSize);

	m_memoryAlignment = (maxHeaderSize % memoryAlignmentSize) ? (memoryAlignmentSize - (maxHeaderSize % memoryAlignmentSize)) : (0);

	m_blockSize = maxHeaderSize + m_memoryAlignment;
	
	m_blocksPerHeap = (m_heapSize - sizeof(Heap)) / m_blockSize;
}

CMemoryMgr::~CMemoryMgr()
{
	DeallocateAll();
}


void* CMemoryMgr::Allocate()
{
	// free list empty, create new page
	if (!m_freeBlockList)
	{
		Heap* newHeap = reinterpret_cast<Heap*>(new char[m_heapSize]);
		++m_numOfBlocks;
		m_numOfBlocks += m_blocksPerHeap;
		m_numOfBlocksFree += m_blocksPerHeap;

		OverWriteHeap(newHeap);

		// page list not empty, link new page
		if (m_heapList)
		{
			newHeap->next = m_heapList;
		}

		//push new page
		m_heapList = newHeap;

		//link new free blocks
		Block* currentBlock = newHeap->block();
		for (unsigned i = 0; i < m_blocksPerHeap - 1; i++)
		{
			currentBlock->next = GetNextBlock(currentBlock);
			currentBlock = GetNextBlock(currentBlock);
		}

		currentBlock->next = nullptr; // last block
		//push new blocks
		m_freeBlockList = newHeap->block();
	}

	//pop free block

	Block* freeBlock = m_freeBlockList;
	m_freeBlockList = m_freeBlockList->next;
	--m_numOfBlocksFree;
	OverWriteAllocatedBlock(freeBlock);
	return freeBlock;
}

void CMemoryMgr::Deallocate(void* pointerToMemory)
{
	//Get memory block
	Block* block = reinterpret_cast<Block*>(pointerToMemory);

	OverWriteBlock(block);

	//put memory block back in free list
	block->next = m_freeBlockList;
	m_freeBlockList = block;
	++m_numOfBlocksFree;
}

void CMemoryMgr::DeallocateAll()
{
	Heap* tempIter = m_heapList;
	while (tempIter)
	{
		Heap* currentHeap = tempIter;
		tempIter = tempIter->next;
		delete[] reinterpret_cast<char*>(currentHeap);
	}

	//Reset
	m_numOfHeaps = 0;
	m_numOfBlocks = 0;
	m_numOfBlocksFree = 0;
	m_heapList = nullptr;
	m_freeBlockList = nullptr;
}

void CMemoryMgr::OverWriteHeap(Heap* heapPointer)
{
	heapPointer->next = nullptr;

	Block* currentBlock = heapPointer->block();
	for (unsigned i = 0; i < m_blocksPerHeap; i++)
	{
		OverWriteBlock(currentBlock);
		currentBlock = GetNextBlock(currentBlock);
	}
}

void CMemoryMgr::OverWriteBlock(Block* blockPointer)
{
	std::memset(blockPointer, FREE_MEMORY_PATTERN, m_blockSize - m_memoryAlignment);

	//For memory alignment
	std::memset(reinterpret_cast<char *>(blockPointer) + m_blockSize - m_memoryAlignment,
		ALIGHTMENT_PATTERN,
		m_memoryAlignment);
}

void CMemoryMgr::OverWriteAllocatedBlock(Block* blockPointer)
{
	std::memset(blockPointer, ALLOCATION_PATTERN, m_blockSize - m_memoryAlignment );

	//For memory alignment
	std::memset(reinterpret_cast<char *>(blockPointer) + m_blockSize - m_memoryAlignment,
		ALIGHTMENT_PATTERN,
		m_memoryAlignment);
}

Block* CMemoryMgr::GetNextBlock(Block* block)
{
	return reinterpret_cast<Block*> (reinterpret_cast<char*>(block) + m_blockSize);
}

