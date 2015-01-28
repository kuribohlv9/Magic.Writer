#pragma once

template <class T>

class ServiceLocator
{
public:
	static void SetService(T* service)
	{
		ms_service = service;
	}

	static T* GetService()
	{
		assert(ms_service);

		return ms_service;
	}

private:
	static T* ms_service;
};

template <class T>
T* ServiceLocator<T>::ms_service = nullptr;