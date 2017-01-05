#include "DataContainer.h"

DataContainer::DataContainer() : m_data()
{
}

DataContainer::~DataContainer()
{
}

void DataContainer::addInt(int v)
{
	m_data.push_back((v >> 24) & 0xff);
	m_data.push_back((v >> 16) & 0xff);
	m_data.push_back((v >> 8) & 0xff);
	m_data.push_back((v >> 0) & 0xff);
}

int DataContainer::getInt()
{
	int v = 0;
	v |= m_data[m_index++] << 24;
	v |= m_data[m_index++] << 16;
	v |= m_data[m_index++] << 8;
	v |= m_data[m_index++] << 0;
	return v;
}

void DataContainer::addUnsignedInt(unsigned int v)
{
	m_data.push_back((v >> 24) & 0xff);
	m_data.push_back((v >> 16) & 0xff);
	m_data.push_back((v >> 8) & 0xff);
	m_data.push_back((v >> 0) & 0xff);
}

unsigned int DataContainer::getUnsignedInt()
{
	unsigned int v = 0;
	v |= m_data[m_index++] << 24;
	v |= m_data[m_index++] << 16;
	v |= m_data[m_index++] << 8;
	v |= m_data[m_index++] << 0;
	return v;
}

void DataContainer::addFloat(float v)
{
	unsigned char* bytes = (unsigned char*)&v;
	m_data.push_back(bytes[0]);
	m_data.push_back(bytes[1]);
	m_data.push_back(bytes[2]);
	m_data.push_back(bytes[3]);
}

float DataContainer::getFloat()
{
	float v = *((float*)&m_data[m_index]);
	m_index += 4;
	return v;
}

void DataContainer::addBool(bool v)
{
	m_data.push_back(v ? 1 : 0);
}

bool DataContainer::getBool()
{
	return m_data[m_index++] != 0;
}

void DataContainer::addString(std::string& v)
{
	for (unsigned int i = 0; i < v.length() + 1; i++) {
		m_data.push_back(v.c_str()[i]);
	}
}

std::string DataContainer::getString()
{
	const int length = strlen((char*)&m_data[m_index]);
	std::string string;
	for (int i = 0; i < length; i++) {
		string.push_back(m_data[m_index + i]);
	}
	m_index += length + 1;
	return string;
}

void DataContainer::addVec3(Vec3& v)
{
	addFloat(v.getX());
	addFloat(v.getY());
	addFloat(v.getZ());
}

Vec3 DataContainer::getVec3()
{
	return Vec3(getFloat(), getFloat(), getFloat());
}

void DataContainer::addData(char* data, int size)
{
	for (int i = 0; i < size; i++) {
		m_data.push_back(data[i]);
	}
}