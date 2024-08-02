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

    XConvertSelection(display, clipboard, utf8_string, property, window, CurrentTime); //#
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
void set_clipboard_content(Display *display, Window window, const char *text) {
    Atom clipboard = XInternAtom(display, "CLIPBOARD", False);
    Atom targets = XInternAtom(display, "TARGETS", False);
    Atom utf8 = XInternAtom(display, "UTF8_STRING", False);
    Atom text_atom = XInternAtom(display, "TEXT", False);
    Atom property = XInternAtom(display, "XSEL_DATA", False);

    XSetSelectionOwner(display, clipboard, window, CurrentTime);
    XSetSelectionOwner(display, XA_PRIMARY, window, CurrentTime);

    XEvent event;
    while (1) {
        XNextEvent(display, &event);

        if (event.type == SelectionRequest) {
            XSelectionRequestEvent *req = &event.xselectionrequest;
            XSelectionEvent ev = {0};

            ev.type = SelectionNotify;
            ev.display = req->display;
            ev.requestor = req->requestor;
            ev.selection = req->selection;
            ev.target = req->target;
            ev.property = req->property;
            ev.time = req->time;

            if (req->target == utf8 || req->target == text_atom) {
                XChangeProperty(req->display, req->requestor, req->property, req->target, 8, PropModeReplace, (unsigned char*)text, strlen(text));
            } else {
                ev.property = None;
            }

            XSendEvent(display, ev.requestor, 0, 0, (XEvent *)&ev);
        }
        
        //TODO add some sleep time here before breakingout of loop
        // if (event.type == SelectionClear) {
        //     break;
        // }
    }
}
int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open display.\n");
        return EXIT_FAILURE;
    }

    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
    XSelectInput(display, window, PropertyChangeMask);

    const char *content = "it's just Rock n'Roll baby";
    set_clipboard_content(display, window,content);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    //return EXIT_SUCCESS;
}