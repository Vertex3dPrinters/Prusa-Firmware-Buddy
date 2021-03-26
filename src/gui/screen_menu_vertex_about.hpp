#pragma once

#include "gui.hpp"
#include "window_header.hpp"
#include "window_text.hpp"
#include "window_icon.hpp"
#include "ScreenFactory.hpp"
#include "screen.hpp"
#include "support_utils.h" //MAX_LEN_4QR

static const constexpr uint16_t PADDING = 10;
static const constexpr uint16_t SCREEN_WIDTH = 240;  //FIXME should be in display.h
static const constexpr uint16_t SCREEN_HEIGHT = 320; //FIXME should be in display.h
static const constexpr uint16_t LINE_HEIGHT = 15;

struct ScreenAboutVertex : public AddSuperWindow<screen_t> {
    ScreenAboutVertex();

private:
    window_header_t header;
    window_icon_t logo_adapted_by;
    window_icon_button_t back_button;
    window_text_t back_label;    
};
