from constants.X11_based_os import xclip_installation_commands
from constants.supported_os import supported_os


def get_install_commands(system_infos : str): 
    getSystem = get_os(system_infos)

def get_os(system_infos:str):
    for os_name in supported_os: 
        if os_name in system_infos:
            return os_name
        

#is_supported_os = lambda os_name: os_name in supported_os 