#pragma once

#define PI 3.14f

// Easing 관련 함수 모음
class CEasingClass
{
public:
	enum E_EasingType
	{
		TYPE_Linear = 0,
		TYPE_QuadIn,
		TYPE_QuadOut,
		TYPE_QuadInOut,
		TYPE_CubicIn,
		TYPE_CubicOut,
		TYPE_CubicInOut,
		TYPE_QuarticIn,
		TYPE_QuarticOut,
		TYPE_QuarticInOut,
		TYPE_QuinticIn,
		TYPE_QuinticOut,
		TYPE_QuinticInOut,
		TYPE_SinIn,
		TYPE_SinOut,
		TYPE_SinInOut,
		TYPE_ExpoIn,
		TYPE_ExpoOut,
		TYPE_ExpoInOut,
		TYPE_CircularIn,
		TYPE_CircularOut,
		TYPE_CircularInOut,
		TYPE_ElasticIn,
		TYPE_ElasticOut,
		TYPE_ElasticInOut,
		TYPE_BounceIn,
		TYPE_BounceOut,
		TYPE_BounceInOut

	};

	//static float GetEasingValue(E_EasingType type, float t, float b, float c, float d = 1)
	//{
	//	switch (type)
	//	{
	//	case TYPE_Linear:	return Linear(t, b, c, d);
	//	case TYPE_QuadIn: 	return QuadIn(t, b, c, d);
	//	case TYPE_QuadOut: 	return QuadOut(t, b, c, d);
	//	case TYPE_QuadInOut: 	return QuadInOut(t, b, c, d);
	//	case TYPE_CubicIn:	return CubicIn(t, b, c, d);
	//	case TYPE_CubicOut:	return CubicOut(t, b, c, d);
	//	case TYPE_CubicInOut:	return CubicInOut(t, b, c, d);
	//	case TYPE_QuarticIn:	return QuarticIn(t, b, c, d);
	//	case TYPE_QuarticOut:	return QuarticOut(t, b, c, d);
	//	case TYPE_QuarticInOut:	return QuarticInOut(t, b, c, d);
	//	case TYPE_QuinticIn:	return QuinticIn(t, b, c, d);
	//	case TYPE_QuinticOut:	return QuinticOut(t, b, c, d);
	//	case TYPE_QuinticInOut:	return QuinticInOut(t, b, c, d);
	//	case TYPE_SinIn:	return SinIn(t, b, c, d);
	//	case TYPE_SinOut:	return sinfOut(t, b, c, d);
	//	case TYPE_SinInOut:	return sinfInOut(t, b, c, d);
	//	case TYPE_ExpoIn:	return ExpoIn(t, b, c, d);
	//	case TYPE_ExpoOut:	return ExpoOut(t, b, c, d);
	//	case TYPE_ExpoInOut:	return ExpoInOut(t, b, c, d);
	//	case TYPE_CircularIn:	return CircularIn(t, b, c, d);
	//	case TYPE_CircularOut:	return CircularOut(t, b, c, d);
	//	case TYPE_CircularInOut:	return CircularInOut(t, b, c, d);
	//	case TYPE_ElasticIn:  	return ElasticEaseIn(t, b, c, d);
	//	case TYPE_ElasticOut: 	return ElasticEaseOut(t, b, c, d);
	//	case TYPE_ElasticInOut: 	return	ElasticEaseInOut(t, b, c, d);
	//	case TYPE_BounceIn: 	return BounceEaseIn(t, b, c, d);
	//	case TYPE_BounceOut: 	return BounceEaseOut(t, b, c, d);
	//	case TYPE_BounceInOut: 	return BounceEaseInOut(t, b, c, d);
	//	}
	//	return 0.0f;
	//}


	static float Linear(float t, float b, float c, float d)
	{
		return float(c * t / d + b);
	}

	static float QuadIn(float t, float b, float c, float d)
	{
		t /= d;
		return float(c * t*t + b);
	}

	static float QuadOut(float t, float b, float c, float d)
	{
		t /= d;
		return float(-c * t*(t - 2) + b);
	}

	static float QuadInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return float(c / 2 * t*t + b);
		t--;
		return float(-c / 2 * (t*(t - 2) - 1) + b);
	}

	static float CubicIn(float t, float b, float c, float d)
	{
		t /= d;
		return float(c * t*t*t + b);
	}

	static float CubicOut(float t, float b, float c, float d)
	{
		t /= d;
		t--;
		return float(c * (t*t*t + 1) + b);
	}

	static float CubicInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t + b;
		t -= 2;
		return float(c / 2 * (t*t*t + 2) + b);
	}

	static float QuarticIn(float t, float b, float c, float d)
	{
		t /= d;
		return float(c * t*t*t*t + b);
	}

	static float QuarticOut(float t, float b, float c, float d)
	{
		t /= d;
		t--;
		return float(-c * (t*t*t*t - 1) + b);
	}

	static float QuarticInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return float(c / 2 * t*t*t*t + b);
		t -= 2;
		return float(-c / 2 * (t*t*t*t - 2) + b);
	}

	static float QuinticIn(float t, float b, float c, float d)
	{
		t /= d;
		return float(c * t*t*t*t*t + b);
	}

	static float QuinticOut(float t, float b, float c, float d)
	{
		t /= d;
		t--;
		return float(c * (t*t*t*t*t + 1) + b);
	}

	static float QuinticInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return float(c / 2 * t*t*t*t*t + b);
		t -= 2;
		return float(c / 2 * (t*t*t*t*t + 2) + b);
	}

	static float SinIn(float t, float b, float c, float d)
	{
		return float(-c * cosf(t / d * (PI / 2)) + c + b);
	}

	static float sinfOut(float t, float b, float c, float d)
	{
		return float(c * sinf(t / d * (PI / 2)) + b);
	}

	static float sinfInOut(float t, float b, float c, float d)
	{
		return float(-c / 2 * (cosf(PI*t / d) - 1) + b);
	}
	//

	static float ExpoIn(float t, float b, float c, float d)
	{
		return float(c * pow(2, 10.0f * (t / d - 1)) + b);
	}

	static float ExpoOut(float t, float b, float c, float d)
	{
		return float(c * (-pow(2, -10 * t / d) + 1) + b);
	}

	static float ExpoInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return float(c / 2 * pow(2, 10 * (t - 1)) + b);
		t--;
		return float(c / 2 * (-pow(2, -10 * t) + 2) + b);
	}

	static float CircularIn(float t, float b, float c, float d)
	{
		t /= d;
		return float(-c * (sqrtf(1 - t * t) - 1) + b);
	}

	static float CircularOut(float t, float b, float c, float d)
	{
		t /= d;
		t--;
		return float(c * sqrtf(1 - t * t) + b);
	}

	static float CircularInOut(float t, float b, float c, float d)
	{
		t /= d / 2;
		if (t < 1) return float(-c / 2 * (sqrtf(1 - t * t) - 1) + b);
		t -= 2;
		return float(c / 2 * (sqrtf(1 - t * t) + 1) + b);
	}

	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float ElasticEaseOut(float t, float b, float c, float d)
	{

		if ((t /= d) == 1)
			return b + c;

		float p = d * .3f;
		float s = p / 4;

		return float(c * pow(2, -10 * t) * sinf((t * d - s) * (2 * PI) / p) + c + b);
	}

	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float ElasticEaseIn(float t, float b, float c, float d)
	{
		if ((t /= d) == 1)
			return b + c;

		float p = d * .3f;
		float s = p / 4.f;

		return float(-(c * pow(2, 10 * (t -= 1)) * sinf((t * d - s) * (2 * PI) / p)) + b);
	}
	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float ElasticEaseInOut(float t, float b, float c, float d)
	{
		if ((t /= d / 2) == 2)
			return float(b + c);

		float p = d * (.3f * 1.5f);
		float s = p / 4;

		if (t < 1)
			return float(-.5f * (c * pow(2, 10 * (t -= 1)) * sinf((t * d - s) * (2 * PI) / p)) + b);
		return float(c * pow(2, -10 * (t -= 1)) * sinf((t * d - s) * (2 * PI) / p) * .5f + c + b);
	}
	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float BounceEaseOut(float t, float b, float c, float d)
	{
		if ((t /= d) < (1 / 2.75f))
			return float(c * (7.5625f * t * t) + b);
		else if (t < (2 / 2.75f))
			return float(c * (7.5625f * (t -= (1.5f / 2.75f)) * t + .75f) + b);
		else if (t < (2.5f / 2.75f))
			return float(c * (7.5625f * (t -= (2.25f / 2.75f)) * t + .9375f) + b);
		else
			return float(c * (7.5625f * (t -= (2.625f / 2.75f)) * t + .984375f) + b);
	}
	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float BounceEaseIn(float t, float b, float c, float d)
	{
		return float(c - BounceEaseOut(d - t, 0, c, d) + b);
	}

	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float BounceEaseInOut(float t, float b, float c, float d)
	{
		if (t < d / 2)
			return BounceEaseIn(t * 2, 0, c, d) * .5f + b;
		else
			return BounceEaseOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
	}

	/*
	 * @descripton http://robertpenner.com/easing/
	 */
	static float BounceEaseOutIn(float t, float b, float c, float d)
	{

		if (t < d / 2)
			return BounceEaseOut(t * 2, b, c / 2, d);
		return BounceEaseIn((t * 2) - d, b + c / 2, c / 2, d);
	}
};

