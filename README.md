# PyCopyBox

**PyCopyBox** is a lightweight tool that allows you to copy multiple items to the clipboard without overwriting previous content. It also provides hotkeys for quick pasting. This tool is built with Python and C.

## How It Works

PyCopyBox interacts with the X11 server on Linux systems to handle selection notifications and selection requests, enabling clipboard operations without relying on third-party libraries. Clipboard history is maintained using a Python deque data structure, and you can access the stored items with custom hotkeys.

### Features

- **Copy Multiple Items**: Store multiple clipboard items without overwriting previous ones.
- **Hotkeys for Fast Pasting**: Use customizable hotkeys to quickly access and paste items from your clipboard history.

### Hotkeys

- **Ctrl+Alt+1**: Paste the first item in the clipboard history.
- **Ctrl+Alt+2**: Paste the second item in the clipboard history.
- And so on..., It supports up to 9 elements. Once you reach 9 elements, the 10th item will overwrite the 1st item, creating a circular buffer:).

### Installation

To install and set up PyCopyBox, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/Mouloud-Msd/PyCopyBox
   cd PyCopyBox
   ```

2. Compile the C program using the provided Makefile:

   ```bash
   make
   ```

3. execute the script:
   ```bash
   NULL
   ```

### Usage

After running the script, you can use the defined hotkeys to paste items from the clipboard history. Copying items will automatically add them to the history.

### Dependencies

- Python 3.x
- X11 libraries
- Make (for compiling the C code)

### Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

### License

This project is licensed under the MIT License.
