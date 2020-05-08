REM Delete Garbage Files
del *.bak
del *.log
del *.pdb
del Draw.lib
del Extensions.lib
del Fonts.lib
del Fps.lib
del Lighting.lib
del Logger.lib
del MatrixOperations.lib
del MultiLanguage.lib
del Sound.lib
del SpeedControl.lib
del Network.lib

del Debug\*.obj
del Debug\*.sbr
del Debug\*.exe
del Debug\*.ilk
del Debug\*.res
del Debug\*.idb
del Debug\*.pdb
del Debug\*.pch
del Debug\*.bsc
del Debug\*.html
del Debug\*.htm
del Debug\*.dep
del Debug\*.manifest

rmdir Debug

del Release\*.obj
del Release\*.sbr
del Release\*.exe
del Release\*.ilk
del Release\*.res
del Release\*.idb
del Release\*.pdb
del Release\*.pch
del Release\*.bsc
del Release\*.html
del Release\*.htm
del Release\*.dep
del Release\*.manifest

rmdir Release

del ..\debug\*.exe
del ..\debug\*.ilk
del ..\debug\*.pdb

rmdir ..\debug

del ..\release\*.exe
del ..\release\*.ilk
del ..\release\*.pdb

rmdir ..\release