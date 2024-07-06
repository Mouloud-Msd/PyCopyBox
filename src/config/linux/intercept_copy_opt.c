#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xfixes.h>
#include <string.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
#include <X11/extensions/XTest.h>

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
    char cmd_Str[200];
    char *path = "index.py";
    snprintf(cmd_Str, sizeof(cmd_Str), "cd ../.. \n python3 \"%s\" %d | xclip -sel clip", path, n);
    //  FILE *pipe2 = popen(cmd_Str, "r");

    //  pclose(pipe2);
    system(cmd_Str);
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
void paste(Display *display) {
    // Assuming XTest extension is available for simulating key events
    KeyCode shift = XKeysymToKeycode(display, XK_Shift_L);
    KeyCode insert = XKeysymToKeycode(display, XK_Insert);

    XTestFakeKeyEvent(display, shift, True, 0); 
    XTestFakeKeyEvent(display,insert, True, 0);    
    XTestFakeKeyEvent(display,insert, False, 0);   
    XTestFakeKeyEvent(display, shift, False, 0); 
    XFlush(display); 
}

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int event_base, error_base;
    if (!XFixesQueryExtension(display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes not supported by X server\n");
        XCloseDisplay(display);
        exit(1);
    }

    int fd[2];
    if(pipe(fd )== -1){
        perror("error opening pipe");
        return 1;
    }

    pid_t pid = fork();
    if(pid == -1){
        printf("Oups error with fok");
        return 1;

    }else if(pid == 0){
        dup2(fd[0], STDIN_FILENO);// std_input content is now fd[0] content
        chdir("../..");
        execl("/usr/bin/python3", "python3", "index.py", "save", NULL);
        perror("excel error");
    }else{
        Window rootWindow = DefaultRootWindow(display);
        XFixesSelectSelectionInput(display, rootWindow, XInternAtom(display, "CLIPBOARD", False), XFixesSetSelectionOwnerNotifyMask); //request notifications when clipboard wner changes


        KeySym event_keys[] = {
            XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9
        };

        int event_keys_size =sizeof(event_keys)/sizeof(event_keys[0]);
        //define window focus
        Window focusWindow;
        int revert;
        XGetInputFocus(display, &focusWindow, &revert);
        // define hotkeys: for by defqult is ctrl+shift+1,2,3,,4....
        for(int i=0 ; i < sizeof(event_keys)/sizeof(event_keys[0]) ; i++){
            XGrabKey(display, XKeysymToKeycode(display, event_keys[i]), ControlMask | Mod1Mask, rootWindow, True, GrabModeAsync, GrabModeAsync);
        }

        XEvent event; // is Union
        int previous_event_timestamp = 0;
        while (1) {
            XNextEvent(display, &event);
            printf("FFF: %d ", event.type);
            if(event.type == KeyPress ){
                KeySym keySym = XLookupKeysym(&(event.xkey), 0 );
                if (is_supported_hotkey(event_keys,event_keys_size,keySym) && (event.xkey.state & ControlMask | Mod1Mask)) {
                    int index_pressed = find_index(event_keys,event_keys_size,keySym); 
                    if(index_pressed != 0){
                        
                        usleep(1000000); // Sleep for 2 seconds (5000000 microseconds)
                        Window focusWindow;
                        int revert;
                        XGetInputFocus(display, &focusWindow, &revert);

                        char flag_str [50];
                        char* code = "paste"; 
                        snprintf(flag_str,sizeof(flag_str), "%s_%d\n", code, index_pressed);
                        
                        
                        write(fd[1], flag_str, strlen(flag_str)); //second writing statement
                        //printf("PPPPPPPPPPP\n");
                        //XFlush(display); 
                        //paste(display);
                        KeyCode shift = XKeysymToKeycode(display, XK_Shift_L);
                        KeyCode insert = XKeysymToKeycode(display, XK_Insert);

                        XTestFakeKeyEvent(display, shift, True, 0); 
                        XTestFakeKeyEvent(display,insert, True, 0);    
                        XTestFakeKeyEvent(display,insert, False, 0);   
                        XTestFakeKeyEvent(display, shift, False, 0); 
                        XFlush(display); 

                        XFlush(display); // Flush the output buffer to ensure events are sent
                    }else{
                        //Throwan error
                        printf("Error hoyKey not supported");
                    }
                }
            }else if (event.type == event_base + XFixesSelectionNotify) {
                XFixesSelectionNotifyEvent* ev = (XFixesSelectionNotifyEvent*)&event; //casting type of &event into XFixesSelectionNotifyEvent
                if (ev->selection == XInternAtom(display, "CLIPBOARD", False)) {
                    //check for previous event timestamp to avoid calling event infinitely when holding keys
                    if (ev->owner != None && ev->owner != DefaultRootWindow(display)) {
                        if(ev->selection_timestamp - previous_event_timestamp > 1000){     
                            previous_event_timestamp =ev->selection_timestamp;
                            char *flag = "copy\n";
                            write(fd[1], flag, strlen(flag)); //first writing statement
                            printf("Clipboard content changed!\n");
                        }
                    }
                }
            } 
            sleep(1);
        }
    
    }

    close(fd[1]);
    close(fd[0]);

    
    XCloseDisplay(display);
    return 0;
}