


//TODO - android systrace

python /mnt/d/Android/platform-tools/systrace/systrace.py \
	gfx input view webview wm am sm rs bionic \
	power pm ss database network adb idle \
	pdx sched irq freq idle disk workq binder_driver binder_lock\
	-a com.xxx.xxx



//TODO - android systrace 示例脚本 bat

@echo on
REM getsystrace [folder=log] [seconds=10] [buffersize=131072]

/* 输出文件名 */
if "%1"=="" (
	set folder=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
) else (
	set folder=%1
)

/* 抓取时间 */
if "%2"=="" (
	set /A seconds=10
) else (
	set /A seconds=%2
)

/* buffer size */
if "%3"=="" (
  set /A buffersize=64072
) else (
  set /A buffersize=%3
)



REM **systrace**.py  -o mytrace.html -t 5 -b 8024 gfx input view webview wm \
	am sm audio video camera memreclaim pagecache binder_lock regulators \
	binder_driver workq sync load hal mmc res dalvik rs bionic power pm ss database \
	network adb vibrator aidl pdx sched irq i2c freq idle disk

mkdir "%folder%"

adb shell **atrace** -c -b %buffersize% -a ecarx.launcher3 --async_start \
	gfx input view webview wm am sm audio video camera hal res dalvik rs \
	binder_driver power pm ss aidl sched freq idle disk sync
REM timeout /T "%seconds%" /NOBREAK
pause
adb shell **atrace** -c -b %buffersize% -a ecarx.launcher3 --async_stop -z gfx input \
	view webview wm am sm audio video camera hal res dalvik rs binder_driver power \
	pm ss aidl sched freq idle disk sync  > "%folder%"/atracedump.atrace


adb shell ps -A             > "%folder%"/ps.txt
adb shell dumpsys package   > "%folder%"/package.txt
REM 注意修改成本地的python path，和本地systrace.py path
adb shell logcat  -d -b all > "%folder%"/bootlog.txt
adb shell dumpsys cpuinfo   > "%folder%"/cpuinfo.txt
adb shell dumpsys meminfo   > "%folder%"/meminfo.txt




//TODO - android systrace 进程状态

RUNNING
RUNNABLE  蓝色
TASK_INTERUPTIBLE      可中断 白色
TASK_UNINTERRUPTIBLE   不可中断 橙色
TASK_KILLABLE          等同于 TASK_WAKEKILL |TASK_UNINTERRUPTIBLE

