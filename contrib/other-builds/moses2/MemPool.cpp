/*
 * MemPool.cpp
 *
 *  Created on: 28 Oct 2015
 *      Author: hieu
 */

#include <boost/foreach.hpp>
#include "MemPool.h"
#include "util/scoped.hh"

using namespace std;

MemPool::Page::Page(std::size_t vSize)
:size(vSize)
{
  mem = (uint8_t*) util::MallocOrThrow(size);
  end = mem + size;
}

MemPool::Page::~Page()
{
	free(mem);
}
////////////////////////////////////////////////////
MemPool::MemPool(size_t initSize)
:m_currSize(initSize)
,m_currPage(0)
,m_count(0)
{
	Page *page = new Page(m_currSize);
	m_pages.push_back(page);

	current_ = page->mem;
	//cerr << "new memory pool";
}

MemPool::~MemPool()
{
  //cerr << "delete memory pool" << endl;
  RemoveAllInColl(m_pages);
}


void *MemPool::More(std::size_t size)
{
	++m_currPage;
	if (m_currPage >= m_pages.size()) {
		// add new page
		m_currSize <<= 1;
		std::size_t amount = std::max(m_currSize, size);

		Page *page = new Page(amount);
		m_pages.push_back(page);

		uint8_t *ret = page->mem;
		current_ = ret + size;
		return ret;
	}
	else {
		// use existing page
		Page &page = *m_pages[m_currPage];
		if (size <= page.size) {
			uint8_t *ret = page.mem;
			current_ = ret + size;
			return ret;
		}
		else {
			// recursive call More()
			return More(size);
		}
	}
}

void MemPool::Reset()
{
	m_currPage = 0;

	if (m_count == 1000) {
		//cerr << "chop ";
		for (size_t i = 0; i < m_pages.size(); ++i) {
			//cerr << i << " ";
		}
		//cerr << endl;
		RemoveAllInColl(m_pages);

		Page *page = new Page(m_currSize);
		m_pages.push_back(page);

		current_ = page->mem;

		m_count = 0;
	}
	else {
		current_ = m_pages[0]->mem;

		++m_count;
	}
}

