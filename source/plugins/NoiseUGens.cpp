/*
	SuperCollider real time audio synthesis system
    Copyright (c) 2002 James McCartney. All rights reserved.
	http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "SC_PlugIn.h"

static InterfaceTable *ft;

struct WhiteNoise : public Unit
{
};

struct ClipNoise : public Unit
{
};

struct BrownNoise : public Unit
{
	float mLevel;
};

struct PinkNoise : public Unit
{
	uint32 mDice[16];
	int32 mTotal;
};

struct Dust : public Unit
{
	float m_density, m_thresh, m_scale;
};

struct Dust2 : public Unit
{
	float m_density, m_thresh, m_scale;
};

struct GrayNoise : public Unit
{
	int32 mCounter;
};

struct Crackle : public Unit
{
	double m_y1, m_y2;
};

struct Logistic : public Unit
{
	double m_y1;
	int mCounter;
};

struct Hasher : public Unit
{
};

struct IRand : public Unit
{
};

struct Rand : public Unit
{
};

struct TRand : public Unit
{
	float m_trig, m_value;
};

struct TIRand : public Unit
{
	float m_trig, m_value;
};

struct NRand : public Unit
{
};

struct LinRand : public Unit
{
};

struct ExpRand : public Unit
{
};

struct CoinGate : public Unit
{
	float m_trig;
};

struct LFClipNoise : public Unit
{
	float mLevel;
	int mCounter;
};

struct LFNoise0 : public Unit
{
	float mLevel;
	int mCounter;
};

struct LFNoise1 : public Unit
{
	float mLevel, mSlope;
	int mCounter;
};

struct LFNoise2 : public Unit
{
	float mLevel, mSlope, mCurve;
	float m_nextvalue, m_nextmidpt;
	int mCounter;
};

struct RandSeed : public Unit
{
	float m_trig;
};

struct RandID : public Unit
{
	float m_id;
};

//////////////////////////////////////////////////////////////////////////////////////////////////


extern "C"
{
	void load(InterfaceTable *inTable);

	void WhiteNoise_next(WhiteNoise *unit, int inNumSamples);
	void WhiteNoise_Ctor(WhiteNoise* unit);

	void GrayNoise_next(GrayNoise *unit, int inNumSamples);
	void GrayNoise_Ctor(GrayNoise* unit);

	void ClipNoise_next(ClipNoise *unit, int inNumSamples);
	void ClipNoise_Ctor(ClipNoise* unit);

	void PinkNoise_next(PinkNoise *unit, int inNumSamples);
	void PinkNoise_Ctor(PinkNoise* unit);

	void BrownNoise_next(BrownNoise *unit, int inNumSamples);
	void BrownNoise_Ctor(BrownNoise* unit);

	void Dust_next(Dust *unit, int inNumSamples);
	void Dust_Ctor(Dust *unit);

	void Dust2_next(Dust2 *unit, int inNumSamples);
	void Dust2_Ctor(Dust2 *unit);

	void Crackle_next(Crackle *unit, int inNumSamples);
	void Crackle_Ctor(Crackle *unit);

	void Hasher_next(Hasher *unit, int inNumSamples);
	void Hasher_Ctor(Hasher *unit);

	void IRand_Ctor(IRand *unit);
	void Rand_Ctor(Rand *unit);
	void LinRand_Ctor(LinRand* unit);
	void NRand_Ctor(NRand* unit);
	void ExpRand_Ctor(ExpRand *unit);

        void CoinGate_Ctor(CoinGate *unit);
	void CoinGate_next_k(CoinGate *unit, int inNumSamples);
	void CoinGate_next(CoinGate *unit, int inNumSamples);
        
	void TIRand_next(TIRand *unit, int inNumSamples);
	void TIRand_Ctor(TIRand *unit);

	void TRand_next(TRand *unit, int inNumSamples);
	void TRand_Ctor(TRand *unit);

	void Logistic_next_1(Logistic *unit, int inNumSamples);
	void Logistic_next_k(Logistic *unit, int inNumSamples);
	void Logistic_Ctor(Logistic *unit);

	void LFClipNoise_next(LFClipNoise *unit, int inNumSamples);
	void LFClipNoise_Ctor(LFClipNoise *unit);

	void LFNoise0_next(LFNoise0 *unit, int inNumSamples);
	void LFNoise0_Ctor(LFNoise0 *unit);

	void LFNoise1_next(LFNoise1 *unit, int inNumSamples);
	void LFNoise1_Ctor(LFNoise1 *unit);

	void LFNoise2_next(LFNoise2 *unit, int inNumSamples);
	void LFNoise2_Ctor(LFNoise2 *unit);
        
	void RandSeed_next(RandSeed *unit, int inNumSamples);
	void RandSeed_Ctor(RandSeed *unit);
        
	void RandID_next(RandID *unit, int inNumSamples);
	void RandID_Ctor(RandID *unit);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// macros to put rgen state in registers
#define RGET \
	RGen& rgen = *unit->mParent->mRGen; \
	uint32 s1 = rgen.s1; \
	uint32 s2 = rgen.s2; \
	uint32 s3 = rgen.s3; 

#define RPUT \
	rgen.s1 = s1; \
	rgen.s2 = s2; \
	rgen.s3 = s3;

//////////////////////////////////////////////////////////////////////////////////////////////////

void ClipNoise_next(ClipNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	
	RGET
	LOOP(inNumSamples, 
		ZXP(out) = fcoin(s1, s2, s3);
	);
	RPUT
}

void ClipNoise_Ctor(ClipNoise* unit)
{
	SETCALC(ClipNoise_next);
	
	ClipNoise_next(unit, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void GrayNoise_next(GrayNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	
	RGET
	int counter = unit->mCounter;

	LOOP(inNumSamples, 
		counter ^= 1L << (trand(s1,s2,s3) & 31);
		ZXP(out) = counter * 4.65661287308e-10f;
	);
	unit->mCounter = counter;
	RPUT
	
}

void GrayNoise_Ctor(GrayNoise* unit)
{
	SETCALC(GrayNoise_next);
	unit->mCounter = 0;
	
	GrayNoise_next(unit, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void WhiteNoise_next(WhiteNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	
	RGET
	LOOP(inNumSamples, 
		ZXP(out) = frand2(s1, s2, s3);
	);
	RPUT
	
}

void WhiteNoise_Ctor(WhiteNoise* unit)
{
	SETCALC(WhiteNoise_next);
	
	WhiteNoise_next(unit, 1);

}


//////////////////////////////////////////////////////////////////////////////////////////////////


void PinkNoise_next(PinkNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	
	RGET
	
	uint32 total = unit->mTotal;
	uint32 *dice = unit->mDice;
	LOOP(inNumSamples, 
		uint32 counter = trand(s1,s2,s3); // Magnus Jonsson's suggestion.
		uint32 newrand = counter >> 13;
		int k = (CTZ(counter)) & 15; 
		uint32 prevrand = dice[k]; 
		dice[k] = newrand; 
		total += (newrand - prevrand); 
		newrand = trand(s1,s2,s3) >> 13;
		uint32 ifval = (total + newrand) | 0x40000000;
		ZXP(out) = ((*(float*)&ifval) - 3.0f); 
		counter ++; 
	);
	unit->mTotal = total;
	RPUT
	
}

void PinkNoise_Ctor(PinkNoise* unit)
{
	SETCALC(PinkNoise_next);
	
	RGET
	uint32 *dice = unit->mDice;
	int32 total = 0;
	for (int i=0; i<16; ++i) {
		uint32 newrand = trand(s1,s2,s3) >> 13;
		total += newrand;
		dice[i] = newrand;
	}	
	unit->mTotal = total;

	ZOUT0(0) = total;
	RPUT
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void BrownNoise_next(BrownNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	RGET
	
	float z = unit->mLevel;
	LOOP(inNumSamples, 
		z += frand8(s1, s2, s3);
		if (z > 1.f) z = 2.f - z; 
		else if (z < -1.f) z = -2.f - z; 
		ZXP(out) = z;
	);
	unit->mLevel = z;
	RPUT
	
}

void BrownNoise_Ctor(BrownNoise* unit)
{
	SETCALC(BrownNoise_next);
	
	unit->mLevel = unit->mParent->mRGen->frand2();

	ZOUT0(0) = unit->mLevel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dust_Ctor(Dust *unit)
{	
	SETCALC(Dust_next);
	
	unit->m_density = 0.f;
	unit->m_scale = 0.f;
	unit->m_thresh = 0.f;
	Dust_next(unit, 1);
}

void Dust_next(Dust *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float density = ZIN0(0);
	float thresh, scale;
	
	RGET
	
	if (density != unit->m_density) {
		thresh = unit->m_thresh = density * unit->mRate->mSampleDur;
		scale = unit->m_scale = thresh > 0.f ? 1.f / thresh : 0.f;
		unit->m_density = density;
	} else {
		thresh = unit->m_thresh;
		scale = unit->m_scale;
	}
	
	LOOP(inNumSamples, 
		float z = frand(s1,s2,s3);
		if (z < thresh) ZXP(out) = z * scale;
		else  ZXP(out) = 0.f;
	);
	
	RPUT
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dust2_Ctor(Dust2 *unit)
{	
	SETCALC(Dust2_next);
	
	unit->m_density = 0.f;
	unit->m_scale = 0.f;
	unit->m_thresh = 0.f;
	Dust2_next(unit, 1);
}

void Dust2_next(Dust2 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float density = ZIN0(0);
	float thresh, scale;
	
	RGET
	
	if (density != unit->m_density) {
		thresh = unit->m_thresh = density * unit->mRate->mSampleDur;
		scale = unit->m_scale = thresh > 0.f ? 2.f / thresh : 0.f;
		unit->m_density = density;
	} else {
		thresh = unit->m_thresh;
		scale = unit->m_scale;
	}
	
	LOOP(inNumSamples, 
		float z = frand(s1,s2,s3);
		if (z < thresh) ZXP(out) = z * scale - 1.f;
		else  ZXP(out) = 0.f;
	);
	
	RPUT
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Crackle_next(Crackle *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float paramf = ZIN0(0);
	float y1 = unit->m_y1;
	float y2 = unit->m_y2;
	float y0;

	LOOP(inNumSamples, 
		ZXP(out) = y0 = fabs(y1 * paramf - y2 - 0.05f);
		y2 = y1; y1 = y0;
	);
	unit->m_y1 = y1;
	unit->m_y2 = y2;
	
}

void Crackle_Ctor(Crackle* unit)
{
	SETCALC(Crackle_next);
	
	unit->m_y1 = 0.3f;
	unit->m_y2 = 0.f;

	Crackle_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Logistic_next_1(Logistic *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	double paramf = ZIN0(1);
	double y1 = unit->m_y1;

	LOOP(inNumSamples, 
		ZXP(out) = y1 = paramf * y1 * (1.0 - y1);	// chaotic equation
	);
	unit->m_y1 = y1;
	
}

void Logistic_next_k(Logistic *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	double paramf = ZIN0(0);
	float freq = ZIN0(1);
	double y1 = unit->m_y1;
	int32 counter = unit->mCounter;
	
	long remain = inNumSamples;
	do { 
		if (counter<=0) {
			counter = (int32)(unit->mRate->mSampleRate / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			y1 = paramf * y1 * (1.0 - y1);	// chaotic equation
		}
		long nsmps = sc_min(counter, remain);
		counter -= nsmps;
		remain -= nsmps;
		LOOP(nsmps, ZXP(out) = y1;);
	} while (remain);
	unit->m_y1 = y1;
	unit->mCounter = counter;
	
}

void Logistic_Ctor(Logistic* unit)
{
	if (INRATE(0) == calc_ScalarRate && ZIN0(0) >= unit->mRate->mSampleRate) {
		SETCALC(Logistic_next_1);
	} else {
		SETCALC(Logistic_next_k);
	}
	
	unit->m_y1 = ZIN0(2);
	
	Logistic_next_1(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Rand_Ctor(Rand* unit)
{	
	float lo = ZIN0(0);
	float hi = ZIN0(1);
	float range = hi - lo;
	RGen& rgen = *unit->mParent->mRGen;	
	ZOUT0(0) = rgen.frand() * range + lo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void TRand_Ctor(TRand* unit)
{	
	float lo = ZIN0(0);
	float hi = ZIN0(1);
	float range = hi - lo;
	RGen& rgen = *unit->mParent->mRGen;	
	ZOUT0(0) = rgen.frand() * range + lo;
	SETCALC(TRand_next);
	unit->m_trig = ZIN0(2);
}

void TRand_next(TRand* unit, int inNumSamples)
{	
	float trig = ZIN0(2);
	if (trig > 0.f && unit->m_trig <= 0.f) {
		float lo = ZIN0(0);
		float hi = ZIN0(1);
		float range = hi - lo;
		RGen& rgen = *unit->mParent->mRGen;	
		ZOUT0(0) = unit->m_value = rgen.frand() * range + lo;
	} else {
		ZOUT0(0) = unit->m_value;
	}
	unit->m_trig = trig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void IRand_Ctor(IRand* unit)
{	
	int lo = (int)ZIN0(0);
	int hi = (int)ZIN0(1);
	int range = hi - lo + 1;
	RGen& rgen = *unit->mParent->mRGen;	
	ZOUT0(0) = (float)(rgen.irand(range) + lo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void TIRand_Ctor(TIRand* unit)
{	
	int lo = (int)ZIN0(0);
	int hi = (int)ZIN0(1);
	int range = hi - lo + 1;
	RGen& rgen = *unit->mParent->mRGen;	
	ZOUT0(0) = (float)(rgen.irand(range) + lo);
	SETCALC(TIRand_next);
	unit->m_trig = ZIN0(2);
}

void TIRand_next(TIRand* unit, int inNumSamples)
{	
	float trig = ZIN0(2);
	if (trig > 0.f && unit->m_trig <= 0.f) {
		int lo = (int)ZIN0(0);
		int hi = (int)ZIN0(1);
		int range = hi - lo + 1;
		RGen& rgen = *unit->mParent->mRGen;	
		ZOUT0(0) = unit->m_value = (float)(rgen.irand(range) + lo);
	} else {
		ZOUT0(0) = unit->m_value;
	}
	unit->m_trig = trig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void CoinGate_Ctor(CoinGate* unit)
{	
	if (unit->mCalcRate == calc_FullRate) {
		SETCALC(CoinGate_next);
	} else {
		SETCALC(CoinGate_next_k);
	}
	unit->m_trig = ZIN0(1);
}

void CoinGate_next_k(CoinGate* unit, int inNumSamples)
{	
	float trig = ZIN0(1);
	float level = 0.f;
	RGen& rgen = *unit->mParent->mRGen;
	if (trig > 0.f && unit->m_trig <= 0.f) {
		
		if(rgen.frand() < ZIN0(0)) {
			level = trig;
		}
	}
    
	ZOUT0(0) = level;
	unit->m_trig = trig;
	
}

void CoinGate_next(CoinGate* unit, int inNumSamples)
{	
	float *trig = ZIN(1);
	float *out = ZOUT(0);
	float level = 0.f;
	float prevtrig = unit->m_trig;
	float probability = ZIN0(0);
	RGen& rgen = *unit->mParent->mRGen;
	LOOP(inNumSamples, 
		float curtrig = ZXP(trig);
		if (prevtrig <= 0.f && curtrig > 0.f) {
			if(rgen.frand() < probability) {
					level = curtrig;
			}
		}
		prevtrig = curtrig;
		ZXP(out) = level; 
	)
	
	unit->m_trig = prevtrig;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void RandSeed_Ctor(RandSeed* unit)
{	
	unit->m_trig = 0.;
	SETCALC(RandSeed_next);
	RandSeed_next(unit, 1);
}

void RandSeed_next(RandSeed* unit, int inNumSamples)
{	
	float trig = ZIN0(0);
        
	if (trig > 0.f && unit->m_trig <= 0.f) {
		RGen& rgen = *unit->mParent->mRGen;
		int seed = (int)ZIN0(1);
		rgen.init(seed);
	} 
	unit->m_trig = trig;
	ZOUT0(0) = 0.f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void RandID_Ctor(RandID* unit)
{	
	 unit->m_id = -1.;
	SETCALC(RandID_next);
	RandID_next(unit, 1);
}

void RandID_next(RandID* unit, int inNumSamples)
{	
	float id = ZIN0(0);
        
	if (id != unit->m_id) {
		unit->m_id = id;
		int iid = (int)id;
		if (iid >= 0 && iid < unit->mWorld->mNumRGens) {
			unit->mParent->mRGen = unit->mWorld->mRGen + iid;
		}
	} 
	ZOUT0(0) = 0.f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void LinRand_Ctor(LinRand* unit)
{	
	float lo = ZIN0(0);
	float hi = ZIN0(1);
	int n = (int)ZIN0(2);
	
	float range = hi - lo;
	RGen& rgen = *unit->mParent->mRGen;	
	float a, b;
	a = rgen.frand();
	b = rgen.frand();
	if (n <= 0) {
		ZOUT0(0) = sc_min(a, b) * range + lo;
	} else {
		ZOUT0(0) = sc_max(a, b) * range + lo;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void NRand_Ctor(NRand* unit)
{	
	float lo = ZIN0(0);
	float hi = ZIN0(1);
	int n = (int)ZIN0(2);
	
	float range = hi - lo;
	RGen& rgen = *unit->mParent->mRGen;	
	float sum = 0;
	for (int i=0; i<n; ++i) {
		sum += rgen.frand();
	}
	ZOUT0(0) = (sum/n) * range + lo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void ExpRand_Ctor(ExpRand* unit)
{	
	float lo = ZIN0(0);
	float hi = ZIN0(1);
	float ratio = hi / lo;
	
	ZOUT0(0) = pow(ratio, unit->mParent->mRGen->frand()) * lo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void Hasher_next(Hasher *unit, int inNumSamples)
{
	float *in = ZIN(0);
	float *out = ZOUT(0);

	LOOP(inNumSamples, 
		union { float f; int i; } u;
		u.f = ZXP(in);
		u.i = 0x40000000 | (Hash(u.i) >> 9);
		ZXP(out) = u.f - 3.f;
	);
	
}

void Hasher_Ctor(Hasher* unit)
{
	SETCALC(Hasher_next);

	Hasher_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void LFClipNoise_next(LFClipNoise *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float freq = ZIN0(0);
	float level = unit->mLevel;
	int32 counter = unit->mCounter;
	RGET
	
	int remain = inNumSamples;
	do {
		if (counter<=0) {
			counter = (int)(unit->mRate->mSampleRate / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			level = fcoin(s1,s2,s3);
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
		LOOP(nsmps, ZXP(out) = level;);
	} while (remain);
	unit->mLevel = level;
	unit->mCounter = counter;
	RPUT
		
}

void LFClipNoise_Ctor(LFClipNoise* unit)
{
	SETCALC(LFClipNoise_next);

	unit->mCounter = 0;
	unit->mLevel = 0.f;
	
	LFClipNoise_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void LFNoise0_next(LFNoise0 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float freq = ZIN0(0);
	float level = unit->mLevel;
	int32 counter = unit->mCounter;
	RGET
	
	int remain = inNumSamples;
	do {
		if (counter<=0) {
			counter = (int32)(unit->mRate->mSampleRate / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			level = frand2(s1,s2,s3);
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
		LOOP(nsmps, ZXP(out) = level;);
	} while (remain);
	unit->mLevel = level;
	unit->mCounter = counter;
	RPUT
		
}

void LFNoise0_Ctor(LFNoise0* unit)
{
	SETCALC(LFNoise0_next);

	unit->mCounter = 0;
	unit->mLevel = 0.f;
	
	LFNoise0_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void LFNoise1_next(LFNoise1 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float freq = ZIN0(0);
	float level = unit->mLevel;
	float slope = unit->mSlope;
	int32 counter = unit->mCounter;
	RGET
	
	int remain = inNumSamples;
	do {
		if (counter<=0) {
			counter = (int32)(unit->mRate->mSampleRate / sc_max(freq, .001f));
			counter = sc_max(1, counter);
			float nextlevel = frand2(s1,s2,s3);
			slope = (nextlevel - level) / counter;
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
		LOOP(nsmps, ZXP(out) = level; level += slope;);
	} while (remain);
	unit->mLevel = level;
	unit->mSlope = slope;
	unit->mCounter = counter;
	RPUT
		
}

void LFNoise1_Ctor(LFNoise1* unit)
{
	SETCALC(LFNoise1_next);

	unit->mCounter = 0;
	unit->mLevel = unit->mParent->mRGen->frand2();
	unit->mSlope = 0.f;
	
	LFNoise1_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void LFNoise2_next(LFNoise2 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float freq = ZIN0(0);
	float level = unit->mLevel;
	float slope = unit->mSlope;
	float curve = unit->mCurve;
	int counter = unit->mCounter;
	RGET
	
	int remain = inNumSamples;
	do {
		if (counter<=0) {
			float value = unit->m_nextvalue;
			unit->m_nextvalue = frand2(s1,s2,s3);
			level = unit->m_nextmidpt;
			unit->m_nextmidpt = (unit->m_nextvalue + value) * .5;

			counter = (int32)(unit->mRate->mSampleRate / sc_max(freq, .001f));
			counter = sc_max(2, counter);
			float fseglen = (float)counter;
			curve = 2.f * (unit->m_nextmidpt - level - fseglen * slope) / (fseglen * fseglen + fseglen);
		}
		int nsmps = sc_min(remain, counter);
		remain -= nsmps;
		counter -= nsmps;
		LOOP(nsmps,
			ZXP(out) = level;
			slope += curve;
			level += slope;
		);
	} while (remain);
	unit->mLevel = level;
	unit->mSlope = slope;
	unit->mCurve = curve;
	unit->mCounter = counter;
	RPUT
		
}

void LFNoise2_Ctor(LFNoise2* unit)
{
	SETCALC(LFNoise2_next);

	unit->mCounter = 0;
	unit->mSlope = 0.f;
	unit->mLevel = 0.f;
	unit->m_nextvalue = unit->mParent->mRGen->frand2();
	unit->m_nextmidpt = unit->m_nextvalue * .5f;
	
	LFNoise2_next(unit, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void WrapBufRd_next0(Unit *unit, int inNumSamples);
void WrapBufRd_next0(Unit *unit, int inNumSamples)
{
	int bufnum = (int)ZIN0(0);
	float *pos = ZIN(1);
		
	SndBuf *buf = unit->mWorld->mSndBufs + bufnum;
	int numchan = buf->channels;
	
	if (numchan != unit->mNumOutputs) {
		ClearUnitOutputs(unit, inNumSamples);
		return;
	}
	float *data = buf->data;
	int numframes = buf->frames;

	float *out[16];
	for (int i=0; i<numchan; ++i) out[i] = OUT(i);

	LOOP(inNumSamples, 
		float fpos = ZXP(pos);
		int ipos = (int)fpos * numchan;
		ipos = sc_mod(ipos, numframes);
		int index = numchan * ipos;
		for (int i=0; i<numchan; ++i) {
			*++(out[i]) = data[index];
			index++;
		}
	);
}

void ClipBufRd_next0(Unit *unit, int inNumSamples);
void ClipBufRd_next0(Unit *unit, int inNumSamples)
{
	int bufnum = (int)ZIN0(0);
	float *pos = ZIN(1);
		
	SndBuf *buf = unit->mWorld->mSndBufs + bufnum;
	int numchan = buf->channels;
	
	if (numchan != unit->mNumOutputs) {
		ClearUnitOutputs(unit, inNumSamples);
		return;
	}
	float *data = buf->data;
	int numframes = buf->frames;
	int maxframe = numframes - 2;

	float *out[16];
	for (int i=0; i<numchan; ++i) out[i] = OUT(i);

	LOOP(inNumSamples, 
		float fpos = ZXP(pos);
		int ipos = (int)fpos * numchan;
		ipos = sc_clip(ipos, 0, maxframe);
		int index = numchan * ipos;
		for (int i=0; i<numchan; ++i) {
			*++(out[i]) = data[index];
			index++;
		}
	);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void load(InterfaceTable *inTable)
{
	ft = inTable;

	DefineSimpleUnit(WhiteNoise);
	DefineSimpleUnit(GrayNoise);
	DefineSimpleUnit(ClipNoise);
	DefineSimpleUnit(PinkNoise);
	DefineSimpleUnit(BrownNoise);
	DefineSimpleUnit(Dust);
	DefineSimpleUnit(Dust2);
	DefineSimpleUnit(Crackle);
	DefineSimpleUnit(Logistic);
	DefineSimpleUnit(Hasher);
	DefineSimpleUnit(LFClipNoise);
	DefineSimpleUnit(LFNoise0);
	DefineSimpleUnit(LFNoise1);
	DefineSimpleUnit(LFNoise2);
	DefineSimpleUnit(Rand);
	DefineSimpleUnit(IRand);
	DefineSimpleUnit(TRand);
	DefineSimpleUnit(TIRand);
	DefineSimpleUnit(NRand);
	DefineSimpleUnit(LinRand);
	DefineSimpleUnit(ExpRand);
	DefineSimpleUnit(CoinGate);
	DefineSimpleUnit(RandSeed);
	DefineSimpleUnit(RandID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
