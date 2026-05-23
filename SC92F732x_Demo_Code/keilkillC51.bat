@echo off
echo 批处理文件正在执行...
rem 这里放置你的命令

setlocal enabledelayedexpansion

REM 使用 dir /ad /b /s 列出所有目录
for /f "delims=" %%i in ('dir /ad /b /s') do (
    REM 获取文件夹名称
    set "foldername=%%~nxi"

    REM 比较文件夹名称是否为 cache 或 Backup（不区分大小写）
    if /i "!foldername!"=="cache" (
        echo 正在删除文件夹 %%i
        rmdir /s /q "%%i"
    ) else if /i "!foldername!"=="Backup" (
        echo 正在删除文件夹 %%i
        rmdir /s /q "%%i"
    )
)

del *.lst /s
del *.obj /s
del *.htm /s
del *.lnp /s
del *.iex /s
del *.__i /s
del *.plg /s
del *.tmp /s

del *.crf /s
del *.o /s
del *.d /s

del *.dep /s   

endlocal

echo 命令执行完成, Press any key to continue . . .
pause

::exit
