mrotate 0.2
===========

Log rotation for Windows

[Russian help](readme-rus.asciidoc)

Console utility for rotating files.

Works on Windows. Installation is not required, there are no dependencies.

Distributed under [GPL license](http://www.gnu.org/licenses/gpl-3.0.html)

**IMPORTANT !!! YOU USE THIS SOFTWARE AT YOUR OWN RISK !!!**

*The author in advance resigns responsibility for all possible damage caused by the program.*

*This program is distributed in the hope that it will be useful,
But WITHOUT ANY WARRANTY; Without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.*


## Description


The program allows you to rotate text log files for programs that do not know how.

Two rotation modes are used.

1. The archive name uses the date / time, old files are deleted if they are older than a certain date. It is used, basically, when the program creates a lot of files, for example by date.
2. The index name is used in the archive name. (The usual behavior of logrotate from linux). Those. The previous file will have the index 001, the older 002, etc. When they are archived, they shift. Files are deleted when the index exceeds the specified number (the number of rotations).
In the case of this mode, file dates are not analyzed. Information about the date of the last rotation is stored in the status file. The default is mrotate.status. You can specify a different file through the command-line option.

The modes are determined automatically, depending on the settings of `Period` and `Size`. If Period is specified by a number, the first mode is used. In other cases, the second one is used (when Period is equal to Daily, Weekly or Monthly, or when the file size is specified). However, you can always set the mode you need manually by specifying `shift=no` for the first mode and `shift=yes` for the second.

Rotation settings are specified via ini configuration text files. (You can also use the format in [java-style](#format-in-java-style)).
The default configuration file is mrotate.ini.

The general scheme of use is as follows: specify the settings for the rotations in the mrotate.ini file and add the daily launch of `mrotate.exe /r` to the scheduler

## Command-line options

* `/help, /h` - Display the list of available keys
* `/rotate, /r` - Run the rotation process for all loaded rotation settings
* `/verify, /v` - Check rotation settings
* `/debug, /d` - Emulation work, the files are not processed. You will see what happens if you start the program with the parameter `/r`
* `/force, /f` - Force rotation, only for entries with `shift = yes`
* `/conf=file, /c=file` - Download the specified file with the rotation settings, the parameter can be specified several times. If the parameter is specified, the file mrotate.ini does not load
* `/arh=file, /a=file` - Load the settings of archiving rules from the specified file, the parameter can be specified several times. In addition to the above, the archivers.ini file is always processed
* `/status=file, /s=file` - Use the specified rotation status file instead of the default

## Configuration file format

The configuration file contains the rotation settings (the usual ini format). UTF-8 encoding

Common file format:

```ini
[Rule_Name1]
Options

[Rule_name2]
Options
```

If you change the name of the entry, you may lose information about the date of the last rotation and an extraordinary rotation will occur. The same when changing the name of the settings file. (Only with shift mode)

### Ini options

* `Source`  
 Directory and mask of rotated files
* `Recurse`  
 Recursively process subdirectories, by default no. If yes is specified, for subdirectories in TargetDir, subdirectories will be created for storage.
* `Shift`  
 The mode of operation is similar to logrotate from linux - i.e. Files have an index, and are shifted. The default is disabled. When specifying this mode, the file name must be constant and will not change.
* `Period`  
  * If a number is specified, the file will rotate if its date from the DateMode parameter is older than the specified number of days. The first rotation mode is used
  * If the period is specified, the second rotation mode is used (only the first letter is checked):
    * `Daily`  
        Daily.
    * `Weekly`  
        Weekly. Usually on Mondays
    * `Monthly`  
        Monthly. Usually on the 1st of the month
* `Size`  
 Process all files larger than the specified size in bytes, it is possible to use the letters K, M. If the `Period` parameter is specified, it has a higher priority and file sizes are not checked. The default is 0. If specified, the second rotation mode is used, which can be changed with the `shift` parameter.
* `Compress`  
 To compress files specified by [archiving rule](#archiving) or not to compress ("no") - there will be a simple renaming
* `TargetDir`  
 The directory for placing old files, if not specified, the archive files are created near to the originals
* `TargetMask`  
 Mask the name of the archives, the archiver's extension is always added to it.
   * For rotation mode 1  
    The default is% FileName. It can also contain date-time parameters that will be replaced by the date / time specified by the DateReplace parameter. If the name is not unique, then in the archive there will be several files.
   * For rotation mode 2 (shift)  
	The default is %FileName%%Index%. Must contain the %Index% parameter. It can contain date/time parameters, but for any date they must have the same length. Ie it is permissible, for example, to specify %FileBaseName.%y%m%d-%H%M.%Index and not valid %FileBaseName.%y-%B-%d-%H%M.%Index because %B (full month name) can have different lengths. Date/time settings are always replaced with the date/time specified by the DateReplace parameter.
* `Keep`  
 How many days to store old files, the analogue of the `Period`, if not specified, then forever. With shift mode, this is the number of stored rotations
* `DateMode`  
 What date to take from files, by default Last.
 Used only when shift mode is off. Pay attention to file dates, they may not be as you expect!  
 Possible (only the first letter is verified):
  * Modify  
   File modification date
  * Created  
   Date of file creation
  * Last  
   The latest of the creation and modification dates. Date closest to current
  * First  
   The earliest of the creation and modification dates.
* `DateReplace`  
 The replacement date in the date parameters (like `%d`), the default Now is the current one, Modify, Created, Last, First are also possible (see the DateMode parameter, only the first letter is checked).
* `Prerotate`  
 The script before rotation is executed once for the entire record
* `Postrotate`  
 The script after rotation is executed once for the entire record
 
## Archiving

Archiving is performed by an external archiver, it must be in %Path% paths or in the current directory.
The name of the archiving rule is specified in the `compress` parameter. (This parameter is required)

The following archiving rules are protected in the program:


Title | Description | Start command
--- | --- | ---
No | Just renaming files | no
7z | Archiving into a 7z container using the PPMD algorithm (for text files) | 7z.exe a %ArhFileName% %FullFileName% -m0=PPMd
7zLzma | 7z archiving according to the LZMA algorithm | 7z.exe a% ArhFileName% FullFileName
Rar | Archiving rar | rar.exe a %ArhFileName% %FullFileName%
WinRar | Backup WinRar | winrar.exe a %ArhFileName% %FullFileName%


Add your own archiving rules by creating an archivers.ini file, about this:
```ini
; The name of the partition is the name of the archiver
; The name must be unique. If you specify a name sewn into the program, it will be redefined.
[7zlzma2]
; The name of the executable file, without specifying the path
; (Although it is possible to specify the full path, but there will not be a search in Path)
ExeName=7z.exe
; Archive File Extension
Extension=.7z
; Arguments of the archiver, are divided by spaces, quotes will not work!
; Instead of the file name, substitute %FullFileName%, instead of the name of the archive %ArhFileName%
Args=a %ArhFileName% %FullFileName% -m0=LZMA2
```

After that, you can use 7zLzma2 in the `compress` parameter, the files will be compressed using the Lzma2

## Examples

* The file test.txt is rotated daily, archiving is performed with the removal of the source file in test.txt.001.7z, test.txt.002.7z, ... The last 10 rotations are stored

```ini
[VerySimple]
Source=D:\temp\rotate\test.txt
Period=Daily
Compress=7z
Keep=10
```

* All *.log files are processed in d:\temp\rotate and subdirectories whose creation date is older than 35 days. They are packed into 7z archives with the name YearMonth.7z, then they are deleted. And in one archive are stored all the files for a month (the date of creation within a month). Archives older than 180 days are deleted.

```ini
[Simple]
Source=d:\temp\rotate\*.log
recurse=yes
Period=35
Compress=7z
TargetDir=d:\temp\rotateold
TargetMask=%Y%m
Keep=180
dateReplace=Create
DateMode=Create
```

* All the *.txt files in d:\temp\rotate and subdirectories with a date older than 90 days are simply deleted.

```ini
[SimpleDelete]
Source=d:\temp\rotate\*.txt
recurse=yes
compress=no
Keep=90
```

* All *.log files are processed in d:\temp\rotate and subdirectories that have a size larger than 100 KB. They are packed into 7z archives with the name FileName.Index.7z (test.log.001.7z, test.log.002.7z ...), and then they are deleted. The last 10 rotations will be stored (Keep=10).

```ini
[ShiftExample]
Source=d:\temp\rotate\*.log
Size=100K 
compress=7z
TargetDir=d:\temp\rotateold
Keep=10
```

* All *.log files are processed in d:\temp\rotate, which have a size larger than 100 KB. They are packed into 7z archives with the name FileNameYearMonthDate.7z (the date is the date of rotation, for example test.log.20111101.7z, test.log.20111102.7z ...), and then they are deleted. Archives are stored 180 days.

```ini
[DateExample]
Source=d:\temp\rotate\*.log
Shift=no
Size=100K
compress=7z
TargetDir=d:\temp\rotateold
TargetMask=%FileName%Y%m%d
Keep=180
```

* All *.log files are processed in d:\temp\rotate. They are daily packed in archives 7z with the name filename.Index.7z (test.log.001.7z, test.log.002.7z ...), and then they are deleted. The last 10 rotations will be stored (Keep = 10).

```ini
[ShiftDailyExample]
Source=d:\temp\rotate\*.log
Period=Daily 
compress=7z
TargetDir=d:\temp\rotateold
Keep=10
```

* All *.log files are processed in d:\temp\rotate. They are packed every 8 days into the archives 7z with the name filename.Index.7z (test.log.001.7z, test.log.002.7z ...), and then they are deleted. The last 10 rotations will be stored (Keep=10).

```ini
[ShiftOutsizeExample]
Source=d:\temp\rotate\*.log
Shift=yes
Period=8 
compress=7z
TargetDir=d:\temp\rotateold
Keep=10
```

## Output messages to a file

By default, all messages are output to the console. Redirecting output to a file (> log.txt) does not work (standard output stream is not used). If you need to write messages to a file, create a logging section in the mrotate.ini file

```
[Logging]
; The log is maintained on the console and in a file
Formatters.f1.class = PatternFormatter
formatters.f1.pattern =%Y-%m-%d %H:%M:%S [%p] %t
Formatters.f1.times = local

Channels.c1.class = ConsoleChannel

Channels.c2.class = FileChannel
Channels.c2.path = mrotate.log
Channels.c2.formatter = f1

; Rotation by size
Channels.c2.rotation = 5M
; Keep the last 5 logs
Channels.c2.purgeCount = 5
; Logs can be compressed by gzip
; Channels.c2.compress = true
; Or rotation by period is possible
; Channels.c2.rotation = weekly


Channels.croot.class = SplitterChannel
Channels.croot.channels = c1,c2

Loggers.root.channel = croot
Loggers.root.level = information
```

You can use any of the logging settings from [Poco](http://pocoproject.org/docs/Poco.FileChannel.html)

## Format in java-style

Instead of the .ini format, you can use the format in java-style. To do this, specify the extension .properties instead of ini.
An example in this format would look like this:

All *.log files are processed in d:\temp\rotate and subdirectories that have a size larger than 100 KB. They are packed into 7z archives with the name FileNameYearMonthDate.7z (the date is the date of rotation, for example test.log.20111101.7z, test.log.20111102.7z ...), and then they are deleted. Archives are stored 180 days.

```
# This is a comment
DateExample.Source=d:\temp\rotate\*.log
DateExample.Shift=no 
DateExample.Size=100K 
DateExample.Compress=7z
DateExample.TargetDir=d:\temp\rotateold
DateExample.TargetMask=%FileName%Y%m%d
DateExample.Keep=180
```

The names register here is important, unlike ini. The entry DateExample.keep=180 will not be read.

## References

The Poco library was used. http://pocoproject.org

The 7-zip archiver can be downloaded for free at http://7-zip.org/


## Contacts

https://github.com/partizand/mrotate

## Annex

Valid parameters in targetMask

### Filenames

  * `%FileName` - Filename
  * `%FileBaseName` - Filename without extension
  * `%FileExt` - File extension (without period)
  * `%Index` - File index, only with shift mode
  
### Dates

* `%w` - abbreviated weekday (Mon, Tue, ...) 
* `%W` - full weekday (Monday, Tuesday, ...) 
* `%b` - abbreviated month (Jan, Feb, ...) 
* `%B` - full month (January, February, ...) 
* `%d` - zero-padded day of month (01 .. 31) 
* `%e` - day of month (1 .. 31) 
* `%f` - space-padded day of month ( 1 .. 31) 
* `%m` - zero-padded month (01 .. 12) 
* `%n` - month (1 .. 12) 
* `%o` - space-padded month ( 1 .. 12) 
* `%y` - year without century (70) 
* `%Y` - year with century (1970) 
* `%H` - hour (00 .. 23) 
* `%h` - hour (00 .. 12) 
* `%a` - am/pm 
* `%A` - AM/PM 
* `%M` - minute (00 .. 59) 
* `%S` - second (00 .. 59) 
* `%s` - seconds and microseconds (equivalent to %S.%F) 
* `%i` - millisecond (000 .. 999) 
* `%c` - centisecond (0 .. 9) 
* `%F` - fractional seconds/microseconds (000000 - 999999) 
* `%z` - time zone differential in ISO 8601 format (Z or +NN.NN) 
* `%Z` - time zone differential in RFC format (GMT or +NNNN) 
* `%%` - percent sign (Actually may not work, e.g. if you write %%FileName - get %Filename)