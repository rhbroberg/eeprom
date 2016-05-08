#pragma once

#include "vmfs.h"
#include <unordered_map>
#include "vmlog.h"

#include "eeprom/PersistentStorage.h"

namespace eeprom
{

class Manager
{
public:

	Manager(const VMUINT base = 0)
	 : _available(base)
	{
	}

	const bool start()
	{
		bool created = create();

		if (created)
		{
			vm_log_info("initializing default values");
			for (const auto & each : _map)
			{
				each.second->setDefault();
			}
		}
		return created;
	}

	void add(PersistentStorage *entry)
	{
		entry->offset(_available);
		_available += entry->size();
		_map[entry->name()] = entry;
	}

	const VM_RESULT eraseAll()
	{
		VM_RESULT status = vm_fs_app_data_delete();
		vm_log_info("deleted persistent storage: %d", status);

		return status;
	}

	PersistentStorage *find(const char *name) const
	{
		auto search = _map.find(name);
		return search != _map.end() ? search->second : NULL;
	}

protected:
	const bool create()
	{
		// ensure file exists; future calls must not use this open attribute, else seeking fails
		VM_FS_HANDLE file;
		bool created = false;

		if ((file = vm_fs_app_data_open(VM_FS_MODE_READ, VM_FALSE)) < 0)
		{
			vm_log_info("creating persistent storage file for the first time");
			file = vm_fs_app_data_open(VM_FS_MODE_CREATE_ALWAYS_WRITE, VM_FALSE);
			created = true;
		}

    	vm_fs_app_data_close(file);
    	return created;
	}

	std::unordered_map<std::string, PersistentStorage *> _map;
	VMUINT _available;
};

}
