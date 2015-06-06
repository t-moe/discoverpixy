Prerequisites:
- Qt5 for Windows (provides C:\Qt\5.4\mingw491_32  and C:\Qt\Tools\mingw491_32)
- Mingw msys-base (provides C:\MinGW\msys\1.0 and make, find )
- Boost (check emulator\libs\boost)

Steps:
- Open up the msys shell using the batchfile
- Navigate to the emulator folder of the project
- Source the qt binaries (qmake, gcc, g++) by executing 
  export PATH=$PATH:/c/Qt/Tools/mingw491_32/bin:/c/Qt/5.4/mingw491_32/bin
- navigate to libs/Pixy and execute make
- navigate back into the emulator folder and execute make
- this should generate you build/emulator.exe

Starting:
- Extract windows_dlls.rar to the build folder
- Copy the emulated folder into the build folder
- Make sure that you installed the pixy usb driver (e.g. by installing pixymon http://cmucam.org/projects/cmucam5/wiki/Latest_release)
- Start emulator.exe

Pitfalls:
- libstdc++ must be provided by qt. do not use one of msys or mingw. you will waste hours with debugging of the make process

Sources:
- libusb for windows was taken from https://github.com/charmedlabs/pixy/tree/master/src/host/windows
- Boost was compiled from sources, using http://andres.jaimes.net/718/how-to-install-the-c-boost-libraries-on-windows/ and http://www.boost.org/doc/libs/1_58_0/more/getting_started/windows.html