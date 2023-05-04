#pragma once

template<typename T>
class Ptr
{
private:
	T* m_Res;

public:
	T* Get() const { return m_Res; }

	T* operator -> () const
	{
		return m_Res;
	}

	void operator = (T* _Res)
	{
		if (nullptr != m_Res)
			m_Res->Release();

		m_Res = _Res;

		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	void operator = (const Ptr<T>& _Res)
	{
		if (nullptr != m_Res)
			m_Res->Release();

		m_Res = _Res.m_Res;

		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	bool operator == (T* _Other)
	{
		return m_Res != _Other;
	}

	bool operator != (T* _Other)
	{
		return m_Res != _Other;
	}

	bool operator == (const Ptr<T>& _Other)
	{
		return m_Res == _Other.m_Res;
	}

	bool operator != (const Ptr<T>& _Other)
	{
		return m_Res != _Other.m_Res;
	}

public:
	Ptr()
		: m_Res(nullptr)
	{

	}

	Ptr(T* _Res)
		: m_Res(_Res)
	{
		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	Ptr(const Ptr<T>& _Res)
		: m_Res(_Res.m_Res)
	{
		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	~Ptr()
	{
		if (nullptr != m_Res)
		{
			m_Res->Release();
		}
	}
};

// 실수 방지를 위해 상수는 보통 왼쪽에 두므로 Ptr이 오른쪽에 있을 때 비교연산자도 구현함
// 비교타입을 T타입으로 두면 null값이 왔을 때는 인식이 불가능하므로 void*로 둠
template<typename T>
bool operator == (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res == _Ptr.Get();
}

template<typename T>
bool operator != (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res != _Ptr.Get();
}