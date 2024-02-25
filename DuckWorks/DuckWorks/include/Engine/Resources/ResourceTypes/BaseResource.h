#pragma once

struct BaseResource
{
	// Make sure to create a virtual destructor to avoid memory leaks
	virtual ~BaseResource() {};

	virtual void LoadFromFile(const String& inFile) = 0;
};
