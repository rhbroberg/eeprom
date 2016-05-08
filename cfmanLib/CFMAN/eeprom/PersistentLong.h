#pragma once

#include "eeprom/PersistentStorage.h"

namespace eeprom
{

class PersistentLong : public PersistentStorage
{
public:

	PersistentLong(const char *name, const long value = 0)
	: PersistentStorage(name)
	, _value(0)
	, _default(value)
	{
	}

	void setValue(const long value)
	{
		if (value != _value)
		{
			_value = value;
			write((void *)&_value, sizeof(_value));
		}
	}

	const long getValue()
	{
		if (!initialized())
		{
			read((void *)&_value, sizeof(_value));
		}
		return _value;
	}

	// not properly used?
	virtual const VMUINT size() const
	{
		return sizeof(_value);
	}

	void setDefault()
	{
		setValue(_default);
	}

protected:
	long _value, _default;
};

}
