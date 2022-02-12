#pragma once
#include "Base.h"

BEGIN(Engine)

class CEasingMgr :public CBase
{
	DECLARE_SINGLETON(CEasingMgr);

private:
	explicit CEasingMgr();
	virtual ~CEasingMgr() = default;

public:


	_float TargetLinear(_float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
	{
		return (fTargetPoint - fStartPoint) * fPassedTime / fTotalTime + fStartPoint;
	}

	_float TargetQuadIn(_float fStartPoint, _float fTargetPoint,  _float fPassedTime, _float fTotalTime)
	{
		fPassedTime /= fTotalTime;
		return (fTargetPoint - fStartPoint) * fPassedTime* fPassedTime* fPassedTime* fPassedTime + fStartPoint;
	}


public:
	virtual void Free() override;

};

END

