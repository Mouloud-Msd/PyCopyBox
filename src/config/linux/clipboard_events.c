#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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
    // char n_to_str[1];
    // snprintf(n_to_str, sizeof(n_to_str), "%d", n);
    n=n-1;
    printf("Oups hotkey pressed\n");
    char cmd_Str[200];
    char *path = "app/ClipBoardManager.py";
    snprintf(cmd_Str, sizeof(cmd_Str), "cd ../.. \n python3 \"%s\" %d | xclip -sel clip", path, n);
     FILE *pipe2 = popen(cmd_Str, "r");

     pclose(pipe2);
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
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int event_base, error_base;

    Window rootWindow = DefaultRootWindow(display);

    XEvent event;
    int previous_event_timestamp = 0;


    KeySym event_keys[] = {
        XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9
    };

    int event_keys_size =sizeof(event_keys)/sizeof(event_keys[0]);
    
    // define hotkeys: for by defqult is ctrl+shift+1,2,3,,4....
    for(int i=0 ; i < sizeof(event_keys)/sizeof(event_keys[0]) ; i++){
        XGrabKey(display, XKeysymToKeycode(display, event_keys[i]), ControlMask | ShiftMask, rootWindow, True, GrabModeAsync, GrabModeAsync);
    }

    while(1){
        XNextEvent(display, &event);
        if(event.type == KeyPress ){
            KeySym keySym = XLookupKeysym(&(event.xkey), 0 );
            if (is_supported_hotkey(event_keys,event_keys_size,keySym) && (event.xkey.state & ControlMask | ShiftMask)) {
                 if(find_index(event_keys,event_keys_size,keySym) != 0){
                     execute_shell_command(find_index(event_keys,event_keys_size, keySym));
                     //printf("OOOOOOOOOOO\n");
                 }else{
                     //Throwan error
                     printf("Error hoyKey not supported");
                 }
            }
        sleep(1);
        }
    }
    XCloseDisplay(display);
    return 0;
}