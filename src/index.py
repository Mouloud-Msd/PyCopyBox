from app.ClipBoardManager import ClipBoardManager
import sys,subprocess
cbm = ClipBoardManager()

if(sys.argv[1] == "save"):
    clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
    decoded_content= clipboard.stdout.decode('UTF-8')
    cbm.add_to_clipboard_history(decoded_content)








#arg = int(sys.argv[1]) 


deque_list = list(cbm.clipboard_history)
print("content:", str(deque_list))
print("length: "+ str(len(deque_list)))

