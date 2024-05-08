# glacier-browser
A custom web browser, powered by Blaze and V8

## Compile instructions
### Warning: this program is still in developement, do not expect it to have anything that works
1. Install CMake, Git and your compiler (ex. Visual Studio)
2. Run this for the debug build
```bash
git clone --recurse-submodules https://github.com/DDev247/glacier-browser.git
cd glacier-browser
mkdir build
cd build
cmake ..
cmake --build .
```
3. To run it, run `build/glacier(.exe)`
