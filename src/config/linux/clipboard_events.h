#ifndef CLIPBOARD_EVENTS_H
#define CLIPBOARD_EVENTS_H

#include <X11/Xlib.h>
#include <stdbool.h>

void sendPaste(Display *display);

bool is_supported_hotkey(const KeySym event_keys[],int size, KeySym target);

#endif