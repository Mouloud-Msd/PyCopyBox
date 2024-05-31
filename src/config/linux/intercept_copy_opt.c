#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/Xfixes.h>
#include <string.h>
#include <sys/wait.h>

void save_clipBoard_content(){
    // char cmd_Str[200];
    // char *path = "index.py";
    // char *flag="save";
    // snprintf(cmd_Str, sizeof(cmd_Str), "cd ../.. \n python3 \"%s\" \"%s\" ", path,flag);
    // system(cmd_Str);
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
        //close(fd[1]);//close writing end of pipe
        dup2(fd[0], STDIN_FILENO);// std_input content is now fd[0] content
        close(fd[0]);//close reading end as well hum hum
        chdir("../..");
        execl("/usr/bin/python3", "python3", "index.py", "save", NULL);
        perror("excel error");
    }else{
        //printf("%d\n", XFixesQueryExtension(display, &event_base, &error_base));
        Window rootWindow = DefaultRootWindow(display);
        XFixesSelectSelectionInput(display, rootWindow, XInternAtom(display, "CLIPBOARD", False), XFixesSetSelectionOwnerNotifyMask); //request notifications when clipboard wner changes

        XEvent event;
        int previous_event_timestamp = 0;
        while (1) {
            printf("%d\n",XFixesSelectionNotify);
            XNextEvent(display, &event);
            if (event.type == event_base + XFixesSelectionNotify) {
                XFixesSelectionNotifyEvent* ev = (XFixesSelectionNotifyEvent*)&event; //casting type of &event into XFixesSelectionNotifyEvent
                if (ev->selection == XInternAtom(display, "CLIPBOARD", False)) {
                    //check for previous event timestamp to avoid calling event infinitely when holding keys
                    if(ev->selection_timestamp - previous_event_timestamp > 1000){     
                        previous_event_timestamp =ev->selection_timestamp;
                        // if (OpenClipboard(NULL)) {
                        //     clip = GetClipboardData(CF_TEXT);
                        //     CloseClipboard();
                        // }
                        char *message = "clip\n";
                        write(fd[1], message, strlen(message));
                        close(fd[1]);
                        printf("Clipboard content changed!\n");
                        save_clipBoard_content();
                        
                    }
                }
            }
            sleep(1);
        }
    
    }

    
    XCloseDisplay(display);
    return 0;
}