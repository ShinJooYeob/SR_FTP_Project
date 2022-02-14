#pragma once

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}


template<typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

template<typename T>
void Safe_Release(T& Temp)
{
	if (Temp)
	{
		Temp->Release();
		Temp = nullptr;
	}
}


static D3DXVECTOR3		GetMouse(void)
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3(float(Pt.x), float(Pt.y), 0.f);
}