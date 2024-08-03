from collections import deque
import sys

class ClipBoardManager():
    copy_counter = 0
    _instance = None
    max_history_list_size=9
    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            print("new instance")
            cls._instance = super().__new__(cls, *args, **kwargs)
            cls.clipboard_history= deque(maxlen=cls.max_history_list_size)
        return cls._instance
    
    @classmethod
    def add_to_clipboard_history(cls,c_content):
        if len(cls.clipboard_history) < 9 :
            cls.clipboard_history.append(c_content)
        else:
            cls.clipboard_history[cls.copy_counter % 9] = c_content
        
        cls.copy_counter+=1

    @classmethod
    def get_specific_content(cls, i):
        ## **if index passed is biger than deque size, then by default it will return last element added to your clipboard history string
        if(i > len(cls.clipboard_history)) : return cls.clipboard_history[len(cls.clipboard_history)- 1] 
        return cls.clipboard_history[i]
        


