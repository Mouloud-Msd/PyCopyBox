from app.ClipBoardManager import ClipBoardManager
import sys,subprocess
cbm = ClipBoardManager()

# if(sys.argv[1] == "save"):
#     clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
#     decoded_content= clipboard.stdout.decode('UTF-8')
#     cbm.add_to_clipboard_history(decoded_content)








#arg = int(sys.argv[1]) 

#do something here so that when something in clipboard content change in c, this code below executes
if(__name__ == "__main__"):
    print("Hello python")

while True:
    deque_list = list(cbm.clipboard_history)
    x = sys.stdin.readline().strip()
    print("X: ", x)
    if(x == "copy"):
        print("COPY !")
        clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
        decoded_content= clipboard.stdout.decode('UTF-8')
        cbm.add_to_clipboard_history(decoded_content)
        print("content:", str(deque_list))
        print("length: "+ str(len(deque_list)))
    else:
        x_list = x.split("_")
        code, index = x_list[0], int(x_list[1])
        if(code== "paste"):
            print("PASTE: ", str(x))
            clipboard_item = cbm.get_specific_content(index - 1)
            # subprocess.run(["echo", clipboard_item,"|" ,"xclip", "selection", "-o"], capture_output=True)

            print("cbm: ",clipboard_item)
            subprocess.run('echo "{}" | xclip -selection clipboard'.format(clipboard_item), shell=True)


