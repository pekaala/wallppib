![Logo](https://raw.githubusercontent.com/pekaala/wallppib/6ab8329be185abf466c69699d1b765b87f1193ae/sc/logo_icon.svg)

# Build Version (Linux)

First, install the necessary dependencies:

- **GTKmm Documentation**: [GTKmm Documentation](https://www.gtk.org/docs/language-bindings/cpp) — More detailed information on GTKmm.

```bash
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libgtkmm-3.0-dev
```

```bash
  git clone https://github.com/username/projeniz.git
  cd wallppib

  mkdir build
  cd build
  cmake ..

  make

  ./wallppib
```

## In Progress Properties

- **GIF Support**: Implementing functionality to handle Gif.
- **UI**:Dynamic UI.
- **Flatpak & Snap (Snappy)**: Support for seamless installation and integration, providing two package distribution methods.
  
- **Localization**: Supporting multiple languages including English, Turkish, and more. (completed)

#### Supported Languages

| Language | Status                                                                                    |
| -------- | ----------------------------------------------------------------------------------------- |
| English  | Available [Help improve translation](https://github.com/pekaala/wallppib/blob/main/src/data/locales/en-US.json)                      |
| Turkish  | Available [Help improve translation](https://github.com/pekaala/wallppib/blob/main/src/data/locales/tr-TR.json)                      |
| Spanish  | Available (translated with AI) [Help improve translation](https://github.com/pekaala/wallppib/blob/main/src/data/locales/es-ES.json) |
| French   | Available (translated with AI) [Help improve translation](https://github.com/pekaala/wallppib/blob/main/src/data/locales/fr-FR.json) |
| German   | Available (translated with AI) [Help improve translation](https://github.com/pekaala/wallppib/blob/main/src/data/locales/de-DE.json) |

![Example GIF](https://github.com/pekaala/wallppib/blob/main/sc/versions/1.1.0/localizationwork.gif)

## Upcoming Features

- **Feedback System**: Allows users to give feedback easily and help improve the application.
- **Pagination and Search**: Efficient search and pagination features for easy navigation through wallpapers.
- **Wallpaper management**: Organize and manage your wallpapers with ease.
- **Customize wallpaper management**: Customization.
- **Build for Windows**: Windows device version.
- **Build for Macos**: Macos device version.
- **OpenGL**: OpenGL & GLFW Support

### Tested

- **Ubuntu**: Fully tested and working.

### Un Tested

- **Windows**: Not working yet.

## Screenshots (for linux)

![Explore.](https://raw.githubusercontent.com/pekaala/wallppib/refs/heads/main/sc/sc_explore.png)

![About.](https://raw.githubusercontent.com/pekaala/wallppib/refs/heads/main/sc/sc_about.png 'sc_about')
