@echo off

REM Set the environment for compiler and buildtools
PATH C:\Projekt_zapper_all\_buildtools;C:\Projekt_zapper_all\_buildtools\gcc-arm-none-eabi;C:\Projekt_zapper_all\_buildtools\gcc-arm-none-eabi\arm-none-eabi;C:\Projekt_zapper_all\_buildtools\gcc-arm-none-eabi\arm-none-eabi\bin;C:\Projekt_zapper_all\_buildtools\gnu;C:\Projekt_zapper_all\_buildtools\openocd;C:\Projekt_zapper_all\_buildtools\openocd\interface;C:\Projekt_zapper_all\_buildtools\gcc-arm-none-eabi\bin;c:\Projekt_zapper_all\_buildtools\gcc-arm-none-eabi\arm-none-eabi\include;


make.exe %*
