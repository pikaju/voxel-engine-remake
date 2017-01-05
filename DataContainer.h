#pragma once

#include <vector>
#include <string>

#include "Vec3.h"

class DataContainer
{
public:
	DataContainer();
	~DataContainer();

	void addInt(int v);
	int getInt();

	void addUnsignedInt(unsigned int v);
	unsigned int getUnsignedInt();

	void addFloat(float v);
	float getFloat();

	void addBool(bool v);
	bool getBool();

	void addString(std::string& v);
	std::string getString();

	void addVec3(Vec3& v);
	Vec3 getVec3();

	inline void reset() { m_index = 0; }
	inline int getIndex() { return m_index; }
	inline void setIndex(int index) { m_index = index; }

	void addData(char* data, int size);
	inline char* getData() { return (char*)m_data.data(); }
	inline unsigned int getSize() { return m_data.size(); }
private:
	std::vector<unsigned char> m_data;
	int m_index;
};

