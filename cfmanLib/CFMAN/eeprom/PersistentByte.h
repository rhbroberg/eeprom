#pragma once

#include "eeprom/PersistentStorage.h"

namespace eeprom
{

class PersistentByte : public PersistentStorage
{
public:

	PersistentByte(const char *name, const VMUINT size, const char *defaultValue = NULL)
	: PersistentStorage(name)
	, _size(size)
	, _default(defaultValue)
	{
		_buf = new char[_size];
	}

	virtual ~PersistentByte()
	{
		delete _buf;
	}

	void setValue(void *buf, const VMUINT length)
	{
		VMUINT safeLength = length < _size ? length : _size;

		// don't write if value is same
		if (memcmp(buf, _buf, safeLength) != 0)
		{
			if (safeLength < _size)
			{
				memset(_buf, 0, _size);
			}
			memcpy(_buf, buf, safeLength);
			// always write out full buffer
			write((void *)_buf, _size);
		}
	}

	void getValue(void *buf, VMUINT *length)
	{
		if (!initialized())
		{
			read((void *)_buf, _size);
		}
		memcpy(buf, _buf, _size);
		*length = _size;
	}

	void *getString()
	{
		if (!initialized())
		{
			read((void *)_buf, _size);
		}
		return _buf;
	}

	void setString(const char *str)
	{
		// include the NULL-termination
		setValue((void *)str, strlen(str) + 1);
		vm_log_info("set string of '%s' to '%s'", name(), str);
	}

	virtual const VMUINT size() const
	{
		return _size;
	}

	virtual void setDefault()
	{
		setValue((void *)_default, _size);
	}

protected:
	const VMUINT _size;
	char *_buf;
	const char  *_default;
};

}
