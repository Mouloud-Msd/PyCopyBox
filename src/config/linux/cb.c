#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

void get_clipboard_content(Display *display, Window window) {
    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    Atom target = XInternAtom(display, "TARGETS", False);
    Atom property = XInternAtom(display, "XSEL_DATA", False);

    XConvertSelection(display, clipboard, utf8_string, property, window, CurrentTime);
    XFlush(display);
    
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == SelectionNotify) {
            if (event.xselection.selection != clipboard) {
                continue;
            }
            if (event.xselection.property) {
                Atom actual_type;
                int actual_format;
                unsigned long nitems;
                unsigned long bytes_after;
                unsigned char *prop = NULL;

                int result = XGetWindowProperty(display, window, property, 0, (~0L), False, AnyPropertyType,
                                                &actual_type, &actual_format, &nitems, &bytes_after, &prop);
                if (result == Success) {
                    if (prop) {
                        printf("Clipboard content: %s\n", prop);
                        XFree(prop);
                    } else {
                        printf("Clipboard is empty or not UTF8_STRING.\n");
                    }
                }
                break;
            }
        }
    }
}
void set_clipboard_content(Display *display, Window window, const char *content){
    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    Atom target = XInternAtom(display, "TARGETS", False);
    Atom property = XInternAtom(display, "XSEL_DATA", False);

    XChangeProperty(display,DefaultRootWindow(display), property, utf8_string,8,PropModeReplace, (unsigned char *)content, strlen(content));

    XSetSelectionOwner(display, clipboard, window,CurrentTime);

    XFlush(display);
}   
int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open display.\n");
        return EXIT_FAILURE;
    }

    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
    XSelectInput(display, window, PropertyChangeMask);

    const char *content = "Hello World !";
    set_clipboard_content(display, window,content);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return EXIT_SUCCESS;
}