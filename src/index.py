from app import ClipBoardManager
import sys
cbm = ClipBoardManager()
cbm.clipboard_histoy.append("MOMO")
arg = int(sys.argv[1])

cbm.add_to_clipboard_history(arg)