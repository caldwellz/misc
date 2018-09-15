rem -fwhole-program breaks multifile programs
windres -o isprime.rco isprime.rc
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -march=i686 -mtune=i686 -Wl,--subsystem,windows -o isprime-dlg.exe isprime-dlg.c primesft.c isprime.rco
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -march=i686 -mtune=i686 -o isprime-con.exe isprime-con.c primesft.c isprime.rco
strip -s *.exe
pause
