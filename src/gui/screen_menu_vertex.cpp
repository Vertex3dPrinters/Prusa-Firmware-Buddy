/*
 * 	screen_menu_vertex.cpp
 *
 *  Created on: 2021.01.31
 *      Author: Juan Francisco Estrada juanfra@cloudhd.eu
 */
//todo THIS SHOULD NOT BE MENU!!!
#include <stdlib.h>

#include "gui.hpp"
#include "screen_menus.hpp"
#include "screen_menu.hpp"
#include "config.h"
#include "version.h"
#include "resource.h"
#include "WindowMenuItems.hpp"
#include "i18n.h"
#include "MItem_menus.hpp"

using Screen = ScreenMenu<EHeader::Off, EFooter::On, HelpLines_None, MI_RETURN, MI_VERTEX_COMMUNITY, MI_VERTEX_GITHUB, MI_VERTEX_WEB, MI_VERTEX_ABOUT>;

class ScreenMenuVertex : public Screen {
public:
    // std::array<char, VERSION_INFO_STR_MAXLEN> version_info_str;
    constexpr static const char *label = N_("VERTEX");
    ScreenMenuVertex();
};

ScreenMenuVertex::ScreenMenuVertex()
    : Screen(_(label)) {
    //=============SCREEN INIT===============
    header.SetIcon(IDR_PNG_info_16px);

}

ScreenFactory::UniquePtr GetScreenMenuVertex() {
    return ScreenFactory::Screen<ScreenMenuVertex>();
}
