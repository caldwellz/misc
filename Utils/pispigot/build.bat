rem -fwhole-program breaks multifile programs

windres -o pispigot-dlg.rco pispigot-dlg.rc
windres -o pispigot-con.rco pispigot-con.rc

gcc -O3 -ffast-math -fomit-frame-pointer -fweb -march=i686 -mtune=i686 -Wl,--reduce-memory-overheads,--subsystem,windows -o pispigot-dlg.exe pispigot-dlg.c pispigot.c pispigot-dlg.rco
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -march=i686 -mtune=i686 -Wl,--reduce-memory-overheads -o pispigot-con.exe pispigot-con.c pispigot.c pispigot-con.rco
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -msse3 -Wl,--reduce-memory-overheads,--subsystem,windows -o pispigot-dlg-sse3.exe pispigot-dlg.c pispigot.c pispigot-dlg.rco
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -msse3 -Wl,--reduce-memory-overheads -o pispigot-con-sse3.exe pispigot-con.c pispigot.c pispigot-con.rco
gcc -O3 -ffast-math -fomit-frame-pointer -fweb -msse3 -Wl,--reduce-memory-overheads -o pispigot-fcon-sse3.exe pispigot-fcon.c pispigot.c pispigot-con.rco
strip -s *.exe
pause
