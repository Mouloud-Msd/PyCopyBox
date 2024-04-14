#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>

int find_index(const KeySym event_keys[],int size, KeySym target){
    for(int i=0; i< size; i++){
        if(event_keys[i] == target) return i+1;
    } 
    return 0;
}

void execute_shell_command(int n){
    /*char n_to_str[1];
    snprintf(n_to_str, sizeof(n_to_str), "%d", n);*/
    char cmd_Str[200];
    snprintf(cmd_Str, sizeof(cmd_Str), "python3 -c 'from src.app.ClipBoardManager import ClipBoardManager; cbm = ClipBoardManager(); cbm.get_content_by_index(%d)' | xclip -sel clip", n);
     FILE *pipe = popen(cmd_Str, "r");

     pclose(pipe);
}

bool is_supported_hotkey(const KeySym event_keys[],int size, KeySym target){
    for(int i=0; i< size; i++){
        if(event_keys[i] == target) return true;
    } 
    return false;
}

void save_copied_content(){
    char *stdString = "python3 -c 'from src.app.ClipBoardManager import ClipBoardManager; cbm = ClipBoardManager(); cbm.add_to_clipboard_history()";
    FILE *pipe = popen(stdString,"r");
    pclose(pipe);
}

int main(){
    Display *display;
    Window rootWindow;
    XEvent event;

    printf("PPPPPPPPPP");

    KeySym event_keys[] = {
        XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9
    };

    int event_keys_size =sizeof(event_keys)/sizeof(event_keys[0]);

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        printf("Error: Unable to open display.") ;
        return 1;
    }

    rootWindow = DefaultRootWindow(display);
    
    //save copied content into deque
    XGrabKey(display, XKeysymToKeycode(display, XK_c), ControlMask | ShiftMask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    
    // define hotkeys: for by defqult is ctrl+shift+1,2,3,,4....
    for(int i=0 ; i < sizeof(event_keys)/sizeof(event_keys[0]) ; i++){
        XGrabKey(display, XKeysymToKeycode(display, event_keys[i]), ControlMask | ShiftMask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    }

    while(1){
    printf("FFF");

         XNextEvent(display, &event);
         if(event.type == KeyPress){
             KeySym keySym = XLookupKeysym(&(event/*  */.xkey), 0 );
             if (is_supported_hotkey(event_keys,event_keys_size,keySym) && (event.xkey.state & ControlMask | ShiftMask)) {
                  if(find_index(event_keys,event_keys_size,keySym) != 0){
                      execute_shell_command(find_index(event_keys,event_keys_size, keySym));
                  }else{
                      //Throwan error
                      printf("Error hoyKey not supported");
                  }

             }
        sleep(1);
        }
    }

    return 0;
}