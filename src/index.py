from config.OS import Os
from config.commands.utility import get_install_commands
Os.initalizeOs()
os = Os()

get_install_commands(os.version)