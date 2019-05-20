#pragma once

#include <istream>
#include <ostream>

#include <osg/Vec3>
#include <osg/Vec3i>
#include <osg/ref_ptr>

#include "vectorOperations.h"

/**
 * Dieser Header bietet Methoden an um die Utilits-Objekte von OSG
 * mithilfe der STD-Streams zu serialisieren und zu deserialisieren
 * Für alle Grafischen Objekte sollten die OSG-eigenen Stream-Operatoren
 * genutzt werden.
 * Funktionen können bei Bedarf ergänzt werden.
 */

inline std::ostream & operator <<(std::ostream & os, const osg::Vec3f & v)
{
	for(int i = 0; i < 3; ++i)
	{
		os << v[i];
		if(i != 2)
			os << std::endl;
	}

	return os;
}


inline std::ostream & operator <<(std::ostream & os, const osg::Vec3i & v)
{
	for(int i = 0; i < 3; ++i)
	{
		os << std::endl << v[i];
		if(i != 2)
			os << std::endl;
	}

	return os;
}


inline std::istream & operator >>(std::istream & is, osg::Vec3f & v)
{
	for(int i = 0; i < 3; ++i)
	{
		float f;
		is >> f;
		v[i] = f;
	}

	return is;
}


inline std::istream & operator >>(std::istream & is, osg::Vec3i & v)
{
	for(int i = 0; i < 3; ++i)
	{
		int f;
		is >> f;
		v[i] = f;
	}

	return is;
}


template<typename T>
void writeBinary(std::ostream & os, const osg::ref_ptr<T> & p)
{
	writeBinary(os, *p);
}


template<typename T>
void readBinary(std::istream & is, osg::ref_ptr<T> & p)
{
	p = new T();
	readBinary(is, *p);
}