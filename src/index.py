from app.ClipBoardManager import ClipBoardManager
import sys
cbm = ClipBoardManager()
arg = int(sys.argv[1]) 

cbm.add_to_clipboard_history(arg)

deque_list = list(cbm.clipboard_histoy)
print(deque_list)
print("length: "+ str(len(deque_list)))

