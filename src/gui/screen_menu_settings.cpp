// screen_menu_settings.c

#include "gui.h"
#include "config.h"
#include "app.h"
#include "marlin_client.h"
#include "screen_menu.hpp"
#include "cmsis_os.h"
#include "sys.h"
#include "eeprom.h"
#include "eeprom_loadsave.h"
#ifdef BUDDY_ENABLE_ETHERNET
    #include "screen_lan_settings.h"
    #include "menu_vars.h"
    #include "wui_api.h"
#endif //BUDDY_ENABLE_ETHERNET
#include "screen_menu_fw_update.h"
#include "filament_sensor.h"
#include "screens.h"
#include "dump.h"
#include "sound_C_wrapper.h"
#include "WindowMenuItems.hpp"
#include "MItem_menus.hpp"
#include "MItem_tools.hpp"

/*****************************************************************************/
//MI_FILAMENT_SENSOR
class MI_FILAMENT_SENSOR : public WI_SWITCH_OFF_ON_t {
    constexpr static const char *const label = "Fil. sens.";

    size_t init_index() const {
        fsensor_t fs = fs_wait_inicialized();
        if (fs == FS_NOT_CONNECTED) //tried to enable but there is no sensor
        {
            fs_disable();
            gui_msgbox("No filament sensor detected. Verify that the sensor is connected and try again.", MSGBOX_ICO_QUESTION);
            fs = FS_DISABLED;
        }
        return fs == FS_DISABLED ? 1 : 0;
    }
    bool fs_not_connected;

public:
    MI_FILAMENT_SENSOR()
        : WI_SWITCH_OFF_ON_t(init_index(), label, 0, true, false) {}
    void CheckDisconnected() {
        fsensor_t fs = fs_wait_inicialized();
        if (fs == FS_NOT_CONNECTED) { //only way to have this state is that fs just disconnected
            fs_disable();
            index = 1;
            gui_msgbox("No filament sensor detected. Verify that the sensor is connected and try again.", MSGBOX_ICO_QUESTION);
        }
    }

protected:
    virtual void OnChange(size_t old_index) {
        old_index == 0 ? fs_disable() : fs_enable();
        fsensor_t fs = fs_wait_inicialized();
        if (fs == FS_NOT_CONNECTED) //tried to enable but there is no sensor
        {
            fs_disable();
            index = old_index;
            gui_msgbox("No filament sensor detected. Verify that the sensor is connected and try again.", MSGBOX_ICO_QUESTION);
        }
    }
};

#ifdef _DEBUG
using parent = screen_menu_data_t<EHeader::Off, EFooter::On, EHelp::Off, MI_RETURN, MI_TEMPERATURE, MI_MOVE_AXIS, MI_DISABLE_STEP,
    MI_FACTORY_DEFAULTS, MI_SERVICE, MI_TEST, MI_FW_UPDATE, MI_FILAMENT_SENSOR, MI_TIMEOUT,
    #ifdef BUDDY_ENABLE_ETHERNET
    MI_LAN_SETTINGS,
    #endif //BUDDY_ENABLE_ETHERNET
    MI_SAVE_DUMP, MI_SOUND_MODE, MI_SOUND_TYPE, MI_HF_TEST_0, MI_HF_TEST_1,
    MI_EE_LOAD_400, MI_EE_LOAD_401, MI_EE_LOAD_402, MI_EE_LOAD_403RC1, MI_EE_LOAD_403,
    MI_EE_LOAD, MI_EE_SAVE, MI_EE_SAVEXML>;
#else
using parent = screen_menu_data_t<EHeader::Off, EFooter::On, EHelp::Off, MI_RETURN, MI_TEMPERATURE, MI_MOVE_AXIS, MI_DISABLE_STEP,
    MI_FACTORY_DEFAULTS, MI_FW_UPDATE, MI_FILAMENT_SENSOR, MI_TIMEOUT,
    #ifdef BUDDY_ENABLE_ETHERNET
    MI_LAN_SETTINGS,
    #endif //BUDDY_ENABLE_ETHERNET
    MI_SAVE_DUMP, MI_SOUND_MODE>;
#endif

class ScreenMenuSettings : public parent {
public:
    constexpr static const char *label = "Settings";
    static void Init(screen_t *screen);
    static int CEvent(screen_t *screen, window_t *window, uint8_t event, void *param);
};

/*****************************************************************************/
//static member method definition
void ScreenMenuSettings::Init(screen_t *screen) {
    Create(screen, label);
}

int ScreenMenuSettings::CEvent(screen_t *screen, window_t *window, uint8_t event, void *param) {
    ScreenMenuSettings *const ths = reinterpret_cast<ScreenMenuSettings *>(screen->pdata);
    if (event == WINDOW_EVENT_LOOP) {
        ths->Item<MI_FILAMENT_SENSOR>().CheckDisconnected();
    }

    return ths->Event(window, event, param);
}

screen_t screen_menu_settings = {
    0,
    0,
    ScreenMenuSettings::Init,
    ScreenMenuSettings::CDone,
    ScreenMenuSettings::CDraw,
    ScreenMenuSettings::CEvent,
    sizeof(ScreenMenuSettings), //data_size
    0,                          //pdata
};
extern "C" screen_t *const get_scr_menu_settings() { return &screen_menu_settings; }
