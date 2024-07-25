from app.ClipBoardManager import ClipBoardManager
import sys,subprocess,asyncio
cbm = ClipBoardManager()

# if(sys.argv[1] == "save"):
#     clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
#     decoded_content= clipboard.stdout.decode('UTF-8')
#     cbm.add_to_clipboard_history(decoded_content)


async def get_clipBoard_content():
    process =  await asyncio.create_subprocess_exec("xclip", "selection", "-o", stdout=asyncio.subprocess.PIPE)
    stdout, stderr = await process.communicate()
    return stdout.decode('UTF-8')




#arg = int(sys.argv[1]) 



async def main():
    while True:
        x = sys.stdin.readline().strip()
        print("X: ", x)
        if(x == "copy"):
            print("COPY !")
            decoded_content =  await get_clipBoard_content()
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

                subprocess.run('echo "{}" | xclip -selection clipboard'.format(clipboard_item), shell=True)


if(__name__ == "__main__"):
    asyncio.run(main())