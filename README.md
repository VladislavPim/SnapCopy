Вот обновлённый README.md с вашим юзернеймом:

```markdown
# SnapCopy

SnapCopy is a lightweight background application for Windows that monitors your clipboard and maintains a searchable history of copied text. It stays conveniently accessible via a sleek, draggable overlay bar.

## Features

- **Clipboard History Tracking** – Automatically saves every text you copy.
- **Quick Access Bar** – A small, draggable overlay bar always on top of other windows.
- **Search History** – Quickly find previously copied items with instant search.
- **Light & Dark Themes** – Choose between light and dark mode to match your preference.
- **Auto-start Option** – Launch SnapCopy automatically when Windows starts.
- **Customizable Font Size** – Adjust the text size for better readability.
- **System Tray Integration** – Minimize to tray for unobtrusive operation.
- **Single Instance** – Prevents multiple instances from running simultaneously.
- **Logging** – Built-in logging for debugging and troubleshooting.

## How It Works

1. SnapCopy runs in the background and listens for clipboard changes using the Windows API.
2. When you copy text (Ctrl+C), it's automatically added to your history.
3. Click the SnapCopy bar (or use the tray icon) to open the history window.
4. Browse, search, or click any item to copy it back to the clipboard.
5. Use the settings window to customize appearance and behavior.

## Installation

### Option 1: Download Pre-built Release (Recommended)

1. Go to the [Releases](https://github.com/VladislavPim/snapcopy/releases) page.
2. Download the latest `SnapCopy.zip` file.
3. Extract the archive to any folder (e.g., `C:\Program Files\SnapCopy`).
4. Run `SnapCopy.exe`.
5. (Optional) Enable auto-start in settings to launch with Windows.

### Option 2: Build from Source

#### Prerequisites

- Windows 10/11 (64-bit)
- [Qt 6.5.3 or later](https://www.qt.io/download) (with MinGW or MSVC compiler)
- [CMake 3.20+](https://cmake.org/download/)
- Visual Studio 2022 (or MinGW) – recommended for Windows development

#### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/VladislavPim/snapcopy.git
   cd snapcopy
   ```

2. Configure CMake:
   - Edit `CMakeLists.txt` and set `CMAKE_PREFIX_PATH` to your Qt installation path, or pass it via command line:
     ```bash
     cmake -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.5.3\msvc2019_64"
     ```

3. Build the project:
   ```bash
   cmake --build build --config Release
   ```

4. After build, run `windeployqt` to copy required Qt DLLs:
   ```bash
   windeployqt --release build/Release/SnapCopy.exe
   ```

5. The executable will be in `build/Release/`.

## Usage Guide

### SnapCopy Bar

- **Drag** – Click and hold anywhere on the bar to move it around the screen.
- **▼ (Expand)** – Opens the history window.
- **⋮ (Settings)** – Opens the settings window.

### History Window

- **Search** – Type in the search box to filter items (case-insensitive).
- **Click an item** – Selects it and enables the "Copy" and "Delete" buttons.
- **Copy** – Copies the selected item back to the clipboard.
- **Delete** – Removes the selected item from history.
- **Clear All** – Deletes all history items.
- **Positioning** – The window automatically opens above or below the bar based on available screen space.

### Settings Window

- **Auto-start** – Run SnapCopy when Windows starts (via registry entry).
- **Font Size** – Adjust the text size (currently applies only to the settings window; history window support planned).
- **Theme** – Switch between Light and Dark mode.

### System Tray

- Right-click the SnapCopy icon in the system tray to show the context menu:
  - **Show History** – Opens the history window.
  - **Exit** – Quits the application.

## Configuration

Settings are stored in the Windows registry under:
```
HKEY_CURRENT_USER\Software\DXStreem\SnapCopy
```

Logs are written to `debug.log` in the application directory (rotates at 5 MB).

## Troubleshooting

### Application won't start
- Check if another instance is already running (only one instance allowed).
- Verify that all required Qt DLLs are present (run `windeployqt` if building manually).
- Check `debug.log` for error messages.

### Clipboard not being tracked
- Ensure SnapCopy is running (check system tray).
- Some applications may use custom clipboard formats; SnapCopy currently handles only plain text (CF_UNICODETEXT).
- Try restarting the application.

### History not saving between sessions
- By design, history is currently stored only in memory and per-session cache files. Full persistence across sessions is planned for future releases.
- Cache files are saved in the `cache` folder next to the executable.

## Building for Release

To create a distributable package:

1. Build the project in Release mode.
2. Run `windeployqt` to collect all necessary Qt dependencies.
3. Include the `platforms` folder and all DLLs.
4. (Optional) Create an installer using tools like Inno Setup or NSIS.

A sample post-build step is included in `CMakeLists.txt` to copy basic DLLs, but `windeployqt` is recommended for completeness.

## Contributing

Contributions are welcome! If you'd like to improve SnapCopy:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/amazing-feature`).
3. Commit your changes (`git commit -m 'Add amazing feature'`).
4. Push to the branch (`git push origin feature/amazing-feature`).
5. Open a Pull Request.

## Known Limitations

- **Windows-only** – Uses WinAPI for clipboard monitoring; not compatible with Linux/macOS.
- **Plain text only** – Doesn't track images or rich text formats.
- **No persistent history** – History is cleared when the application exits (cache files exist but aren't reloaded).
- **Font size setting** – Currently only affects the settings window, not the history list.

## Roadmap

- [ ] Persistent history storage (SQLite)
- [ ] Support for images in clipboard
- [ ] Keyboard shortcuts (e.g., Ctrl+Shift+V to open history)
- [ ] Export/import history
- [ ] Multi-language support
- [ ] Improved theme customization

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built with [Qt 6](https://www.qt.io/)
- Inspired by clipboard managers like Ditto and CopyQ
- Thanks to all contributors and testers

## Contact

Project Link: [https://github.com/VladislavPim/snapcopy](https://github.com/VladislavPim/snapcopy)

---

**SnapCopy** – Never lose your clipboard history again!
```
