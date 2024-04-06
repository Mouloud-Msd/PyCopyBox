from collections import deque

class ClipBoardManager():
    def __init__(self,max_history_list_size=10):
        self.clipboard_histoy= deque(maxlen=max_history_list_size)
    
    def add_to_clipboard_history(self,c_content):
        if len(self.clipboard_histoy) <= 10 :
            self.clipboard_content.append(c_content)