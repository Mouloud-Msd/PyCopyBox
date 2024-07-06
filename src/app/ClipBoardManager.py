from collections import deque
import sys

class ClipBoardManager():
    copy_counter = 0
    _instance = None
    max_history_list_size=10

    # def __init__(self, max_history_list_size=None):
    #     if max_history_list_size is not None:
    #         self.max_history_list_size = max_history_list_size
    
    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            print("new instance")
            cls._instance = super().__new__(cls, *args, **kwargs)
            cls.clipboard_history= deque(maxlen=cls.max_history_list_size)
        return cls._instance
    
    @classmethod
    def add_to_clipboard_history(cls,c_content):
        if len(cls.clipboard_history) < 10 :
            cls.clipboard_history.append(c_content)
        else:
            cls.clipboard_history[cls.copy_counter % 10] = c_content
        
        cls.copy_counter+=1

    @classmethod
    def get_specific_content(cls, i):
        return cls.clipboard_history[i]
        


