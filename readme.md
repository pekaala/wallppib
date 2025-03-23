![Logo.](https://raw.githubusercontent.com/pekaala/wallppib/222405ba3c955a9a8abe7fec8b3b14435c991457/sc/logo_icon.svg)


# Build Version (Linux)

First, install the necessary dependencies:


- **GTKmm Documentation**: [GTKmm Documentation](https://www.gtk.org/docs/language-bindings/cpp) — More detailed information on GTKmm.

```bash
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libgtkmm-3.0-dev
```

  ```bash
    git clone https://github.com/pekaala/wallppib.git
    cd wallppib
    
    mkdir build
    cd build
    cmake ..
    
    make
    
    ./wallppib
  ```



## In Progress Properties
-  **Localization**: Supporting multiple languages including English, Turkish, and more.
- **UI**:Dynamic UI.
- **GIF Support**: Implementing functionality to handle Gif.


    
## Upcoming Features
- **Feedback System**: Allows users to give feedback easily and help improve the application.
- **Flatpak & Snap (Snappy)**: Support for seamless installation and integration, providing two package distribution methods.
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



![About.](https://raw.githubusercontent.com/pekaala/wallppib/refs/heads/main/sc/sc_about.png "sc_about")

