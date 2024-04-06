from ..OS import Os
from constants.x11_based_os import x11_based_systems
from constants.clipboard_history import history
import subprocess

def get_install_commands(system_infos : str) -> str: 
    system_name = get_os(system_infos)
    
    return x11_based_systems[system_name]


def get_os(system_infos:str):
    for os_name in x11_based_systems: 
        if os_name in system_infos:
            return os_name
        
def install_xclip():
    cmd = get_install_commands(os.version)
    cmd_list = strCmd_into_list(cmd)
    execute_cmd(cmd_list)

        
def get_clipboard_content():
    try:
        clipboard =  subprocess.run(["xclip", "selection", "-o"], capture_output=True)
        return clipboard.stdout.decode('utf-8')
    except FileNotFoundError as e :
        print("Oups Xclip not installed on your machine. \nXclip is needed and should be installed.")
        install_xclip()


def save_clipboard_history():
    old_clipboard_content = history[-1]
    actual_clipboard_content=get_clipboard_content()
    if old_clipboard_content != None and old_clipboard_content != actual_clipboard_content :
        history.append(actual_clipboard_content)



strCmd_into_list = lambda cmd: cmd.split(" ") 
execute_cmd = lambda cmd_list:subprocess.run(cmd_list)

Os.initalizeOs()
os = Os()