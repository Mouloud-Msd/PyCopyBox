#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <unistd.h>
#include <vector>
#include <algorithm> 

using namespace std;

int find_index(const vector<KeySym> event_keys, KeySym target){
    auto it = find(event_keys.begin(), event_keys.end(), target);
    if(it != event_keys.end()){
        int index = distance(event_keys.begin(), it);
        return index + 1;
    }
    return 0;
}


 void execute_shell_command(int n){
     //string stdString = "pyton3 get_deque_content " + to_string(n) + " | xclip -selection";
    //string stdString = "echo POPO" + to_string(n) + " | xclip -sel clip";
    string stdString = "echo POPO | xclip -sel clip";


     FILE *pipe = popen(stdString.c_str(), "r");

     pclose(pipe);
 }


bool is_supported_hotkey(const vector<KeySym> event_keys, KeySym target){
    int occ = count(event_keys.begin(),event_keys.end(), target);
    if(occ > 0){
        return true;
    }
    return false;
}

int main() {
    Display *display;
    Window rootWindow;
    XEvent event;

    vector<KeySym> event_keys = {
        XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9
    };

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        cerr << "Error: Unable to open display." << endl;
        return 1;
    }

    rootWindow = DefaultRootWindow(display);
    
    // define hotkeys: for by defqult is ctrl+shift+1,2,3,,4....
    for(int i=0 ; i < event_keys.size() ; i++){
        XGrabKey(display, XKeysymToKeycode(display, event_keys[i]), ControlMask | ShiftMask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    }

    while (true) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            KeySym keySym = XLookupKeysym(&event.xkey, 0);
            if (is_supported_hotkey(event_keys,keySym) && (event.xkey.state & ControlMask | ShiftMask)) {
                cout << "find index: "  + find_index(event_keys,keySym) << endl;
                execute_shell_command(find_index(event_keys,keySym));
                cout << "Hotkey pressed" << keySym << endl;
            }
        }
        usleep(100000); // let the cpu rest:)
    }

    XCloseDisplay(display);

    return 0;
}

