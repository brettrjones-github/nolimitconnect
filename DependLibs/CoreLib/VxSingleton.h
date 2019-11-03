//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================
#pragma once
//! implement singleton
template<typename T> class VxSingleton 
{
public:
	static T &Instance() 
	{
		static T VxSingletonInstance;
		return VxSingletonInstance;
	}
private:
	//=== constructor ===//
	VxSingleton();
	//=== constructor ===//
	VxSingleton(VxSingleton const &Object);
	//=== destructor ===//
	~VxSingleton();
	//=== operator ===//
	VxSingleton &operator=(VxSingleton const &Object);
};
