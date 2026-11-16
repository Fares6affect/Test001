#pragma once
#include "IslandStrategy.h"

struct ImproverConfig {
	double uniteProbability;
	double mutateProbability;

	double mainSelProb;

	double mConstProb;
	double mVarProb;
	double mFuncProb;
	double mSubTreeProb;
	double mDelProb;
	double mInsProb;
	double mSwapProb;

	ImproverConfig(
		double uniteProbability,
		double mutateProbability,

		double mainSelProb,

		double mConstProb,
		double mVarProb,
		double mFuncProb,
		double mSubTreeProb,
		double mDelProb,
		double mInsProb,
		double mSwapProb
		):
		uniteProbability(uniteProbability),
		mutateProbability(mutateProbability),

		mainSelProb(mainSelProb),

		mConstProb(mConstProb),
		mVarProb(mVarProb),
		mFuncProb(mFuncProb),
		mSubTreeProb(mSubTreeProb),
		mDelProb(mDelProb),
		mInsProb(mInsProb),
		mSwapProb(mSwapProb)
	{}
};

ImproverConfig getImproverConfig(IslandStrategy strategy);