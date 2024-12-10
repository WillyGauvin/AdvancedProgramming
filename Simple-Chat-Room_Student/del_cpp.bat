FOR /d /r . %%d IN (".vs") DO @IF EXIST "%%d" rd /s /q "%%d"
FOR /d /r . %%d IN ("x64") DO @IF EXIST "%%d" rd /s /q "%%d"
del /s /q *.pch
