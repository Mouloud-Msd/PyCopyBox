from collections import deque
import sys

class ClipBoardManager():
    copy_counter = 0
    def __init__(self,max_history_list_size=10):
        self.clipboard_histoy= deque(maxlen=max_history_list_size)
    
    def add_to_clipboard_history(self,c_content):
        print("add content !\n")
        if len(self.clipboard_histoy) < 10 :
            self.clipboard_histoy.append(c_content)
        else:
            self.clipboard_histoy[self.copy_counter % 10] = c_content
        
        self.copy_counter+=1
        


