from app.ClipBoardManager import ClipBoardManager
import sys,subprocess,asyncio
cbm = ClipBoardManager()

# if(sys.argv[1] == "save"):
#     clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
#     decoded_content= clipboard.stdout.decode('UTF-8')
#     cbm.add_to_clipboard_history(decoded_content)


async def get_clipBoard_content(handler_executable):
    clipboard =  subprocess.run([handler_executable,"get"], capture_output=True)
    return clipboard.stdout.decode('UTF-8')

async def set_clipBoard_content(handler_executable, clipboard_item):
    subprocess.run([handler_executable,"set", clipboard_item], capture_output=True)



#arg = int(sys.argv[1]) 



async def main():
    handler_executable = "../clipboard_handler"
    while True:
        x = sys.stdin.readline().strip()
        print("X: ", x)
        if(x == "copy"):
            print("COPY !")
            decoded_content =  await get_clipBoard_content(handler_executable)
            cbm.add_to_clipboard_history(decoded_content)
            deque_list = list(cbm.clipboard_history)
            print("content:", str(deque_list))
            print("length: "+ str(len(deque_list)))
        else:
            x_list = x.split("_")
            code, index = x_list[0], int(x_list[1])
            if(code== "paste"):
                print("PASTE: ", str(x))
                clipboard_item = cbm.get_specific_content(index - 1)
                await set_clipBoard_content(handler_executable, clipboard_item)


if(__name__ == "__main__"):
    asyncio.run(main())