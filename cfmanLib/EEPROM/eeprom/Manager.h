#pragma once

#include <map>

#include "eeprom/PersistentStorage.h"

namespace eeprom
{

class Manager
{
public:

	Manager(const VMUINT base = 0);

	const bool start();
	void add(PersistentStorage *entry);
	const VM_RESULT eraseAll();
	PersistentStorage *find(const char *name) const;

protected:
	const bool create();

	std::map<std::string, PersistentStorage *> _map;
	VMUINT _available;
};

}
