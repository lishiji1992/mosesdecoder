/*
 * Search.h
 *
 *  Created on: 16 Nov 2015
 *      Author: hieu
 */

#pragma once
#include <boost/pool/pool_alloc.hpp>
#include "../Search.h"
#include "../CubePruning/Misc.h"
#include "Stacks.h"
#include "../../legacy/Range.h"

namespace Moses2
{

class Bitmap;
class Hypothesis;
class InputPath;
class TargetPhrases;

namespace NSCubePruningPerMiniStack
{

class Search : public Moses2::Search
{
public:
	Search(Manager &mgr);
	virtual ~Search();

	virtual void Decode();
	const Hypothesis *GetBestHypothesis() const;

protected:
	Stacks m_stacks;

	NSCubePruning::CubeEdge::Queue m_queue;
	NSCubePruning::CubeEdge::SeenPositions m_seenPositions;

	// CUBE PRUNING VARIABLES
	// setup
	MemPoolAllocator<NSCubePruning::CubeEdge*> m_cubeEdgeAlloc;
	typedef std::vector<NSCubePruning::CubeEdge*, MemPoolAllocator<NSCubePruning::CubeEdge*> > CubeEdges;
	std::vector<CubeEdges*> m_cubeEdges;

	std::deque<NSCubePruning::QueueItem*> m_queueItemRecycler;

	// CUBE PRUNING
	// decoding
	void Decode(size_t stackInd);
	void PostDecode(size_t stackInd);
	void Prefetch(size_t stackInd);
};

}

}

