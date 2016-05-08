#pragma once

#include "string.h"
#include "vmmemory.h"
#include "vmfs.h"

namespace eeprom
{

#define MAX_NAME_LENGTH 16

class PersistentStorage
{
public:

	PersistentStorage(const char *name)
	: _offset(0)
	, _initialized(false)
	{
		VMUINT safeLength = (strlen(name) + 1 < MAX_NAME_LENGTH) ? (strlen(name) + 1) : MAX_NAME_LENGTH;
		memcpy((void *)_name, (void *)name, safeLength);
		_name[MAX_NAME_LENGTH - 1] = 0;
	}

	virtual ~PersistentStorage()
	{

	}

	const char *name() const
	{
		return _name;
	}

	const VMUINT offset() const
	{
		return _offset;
	}

	void offset(const VMUINT value)
	{
		_offset = value;
	}

	const bool initialized() const
	{
		return _initialized;
	}

	virtual const VMUINT size() const = 0;
	virtual void setDefault() = 0;

protected:
	void write(void *buf, const VMUINT length)
	{
        VM_FS_HANDLE file = vm_fs_app_data_open(VM_FS_MODE_WRITE, VM_FALSE);
    	VMUINT written;

    	VM_RESULT status = vm_fs_app_data_seek(file, _offset, VM_FS_BASE_BEGINNING);
    	vm_log_info("moved pointer to %d: %d", _offset, status);
    	if (status == -1)
    	{
        	vm_fs_app_data_close(file);
    		extend(length);
            file = vm_fs_app_data_open(VM_FS_MODE_WRITE, VM_FALSE);
        	status = vm_fs_app_data_seek(file, _offset, VM_FS_BASE_BEGINNING);
        	vm_log_info("post-extend moved pointer to %d: %d", _offset, status);
    	}
    	status = vm_fs_app_data_write(file, buf, length, &written);
    	vm_log_info("wrote %d bytes to '%s'", written, name());
    	vm_fs_app_data_close(file);

		_initialized = true;
	}

	void read(void *buf, const VMUINT length)
	{
        VM_FS_HANDLE _file = vm_fs_app_data_open(VM_FS_MODE_READ, VM_FALSE);
    	VMUINT written;

    	// if seek fails fail operation
    	VM_RESULT status = vm_fs_app_data_seek(_file, _offset, VM_FS_BASE_BEGINNING);
    	vm_log_info("moved pointer to %d: %d", _offset, status);
        if (status == -1)
        {
        	vm_fs_app_data_close(_file);
        	extend(length);
            // file = vm_fs_app_data_open(VM_FS_MODE_READ, VM_FALSE);
            _file = vm_fs_app_data_open(VM_FS_MODE_READ, VM_FALSE);
            status = vm_fs_app_data_seek(_file, _offset, VM_FS_BASE_BEGINNING);
        	vm_log_info("post-extend moved pointer to %d: %d", _offset, status);
        }

    	status = vm_fs_app_data_read(_file, buf, length, &written);
    	vm_log_info("read %d bytes from '%s'", written, name());
    	vm_fs_app_data_close(_file);

    	_initialized = true;
	}

	void extend(const VMUINT length)
	{
		vm_log_info("auto-extending storage for object '%s'", name());
        VM_FS_HANDLE file = vm_fs_app_data_open(VM_FS_MODE_WRITE, VM_FALSE);
    	VM_RESULT end = vm_fs_app_data_seek(file, 0, VM_FS_BASE_END);

    	if (end != -1)
    	{
    		VMUINT written;
    		VMUINT filler = _offset + length - end;
    		vm_log_info("extending: end is %d, offset is %d, length %d, sum is %d", end, _offset, length, filler);
    		char *empty = new char[filler];
    		memset(empty, 0, filler);
        	VM_RESULT status = vm_fs_app_data_write(file, empty, filler, &written);
        	vm_log_info("extended %d empty bytes to '%s'", written, name());
        	delete [] empty;
    	}
    	else
    	{
    		vm_log_info("cannot seek to end? help!");
    	}
    	vm_fs_app_data_close(file);
	}

	bool _initialized;
	char _name[MAX_NAME_LENGTH];
	VMUINT _offset;
};

}
