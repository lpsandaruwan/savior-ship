# savior-ship

An opensource space ship fighter game using SDL2.


### Building

Builds have been tested on both Linux and Windows 10.


#### Build Requirements

- gcc g++, clang or MSVC Compiler (depending upon your operating system)


#### Build Dependencies

- sdl2
- sdl2_image
- sdl2_ttf
- sdl2_mixer

Linux users can download sdl2 from [https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php) but it's recommended to instead build against the software available from the operating system's package manager instead (apt-get or dnf for example.)


#### Building using Windows 10

There is a build script inside of the **windows** directory which will download and build all of the dependencies for you using [vcpkg][6b7f4002]. Please open and read the file before running it. There is some software you need to install first and two variables you will need to set to ensure that the script will run cleanly.

  [6b7f4002]: https://github.com/Microsoft/vcpkg "vcpkg"

Once the variables have been set correctly, it should be as simple as opening a command prompt, moving into the directory and
issuing the command:

```
git clone https://github.com/lpsandaruwan/savior-ship.git
cd savior-ship\windows

build.cmd
```

Once the project has sucessfully built, you can then make your changes to the source code and run **build.cmd** again. Doing so will quickly rebuild the project's binaries for you.

#### Build in Linux

Clone repository and using cmake do,
```
git clone https://github.com/lpsandaruwan/savior-ship.git
cd savior-ship

cmake CMakeLists.txt
```

Now build and install,
```
make
sudo make install
```

From application launcher menu search for "Savior Ship".

Enjoy...!
