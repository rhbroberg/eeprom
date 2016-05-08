#include "vmtype.h" 
#include "vmboard.h"
#include "vmsystem.h"
#include "vmlog.h" 
#include "vmcmd.h" 
#include "vmdcl.h"
#include "vmdcl_gpio.h"
#include "vmthread.h"
#include "vmres.h"

#include "cfmanLib.h"

#include "eeprom/Manager.h"
#include "eeprom/PersistentByte.h"
#include "eeprom/Persistent.h"

eeprom::Manager myMan;

void handle_sysevt(VMINT message, VMINT param) 
{
    switch (message) 
    {
    case VM_EVENT_CREATE:
    {
    	eeprom::PersistentByte frist("first", 8);
    	eeprom::Persistent<long> second("second");
    	eeprom::Persistent<long> third("more please", 99);
    	eeprom::PersistentByte fourth("coolio", 16, "corge");
    	eeprom::Persistent<long> fifth("bad");
    	eeprom::Persistent<long> sixth("foo");
    	eeprom::Persistent<char> seven("7", 7);
    	eeprom::Persistent<char> eight("8", 8);

    	myMan.add(&frist);
    	myMan.add(&second);
    	myMan.add(&third);
    	myMan.add(&fourth);
    	myMan.add(&fifth);
    	myMan.add(&sixth);
    	myMan.add(&seven);
    	myMan.add(&eight);

//#define WRITEME
#ifdef WRITEME
    	myMan.eraseAll();
    	myMan.start();

    	const char *foo = "xyz";
    	frist.setString(foo);
    	second.setValue(42);

    	vm_log_info("read back %s and %d, %d and %s", frist.getString(), second.getValue(), third.getValue(), fourth.getString());
    	vm_log_info("templates say %d and %d", seven.getValue(), eight.getValue());
#else
    	myMan.start();

    	vm_log_info("read back %s and %d, %d and %s", frist.getString(), second.getValue(), third.getValue(), fourth.getString());
    	fourth.setString("hello again");

//    	eeprom::PersistentByte *found = dynamic_cast<eeprom::PersistentByte *>(myMan.find("coolio"));
    	eeprom::PersistentByte *found = (eeprom::PersistentByte *)(myMan.find("coolio"));
    	if (found)
    	{
    		vm_log_info("looked up '%s' and found '%s'", found->name(), found->getString());
    	}

    	sixth.setValue(48);
    	fifth.setValue(10);
    	vm_log_info("getting foo value: %d", sixth.getValue());
    	vm_log_info("templates say %d and %d", seven.getValue(), eight.getValue());

#endif

        /* Create sub thread for Blink PIN D8 in loop */
        /* Otherwise you need to blink them in a timer callback, never blocking main thread. */

        break;
    }
	case VM_EVENT_PAINT:	
        /* the message of asking for application to repaint screen */
		/* cerate base layer that has same size as the screen */
        break;

    case VM_EVENT_QUIT:
        break;
    }
}

extern "C"
{
/* Entry point */
void vm_main(void)
{
    /* register system events handler */
    vm_pmng_register_system_event_callback(handle_sysevt);
    /* Init APP resource */
    vm_res_init(0);
}

}

