mrotate 0.2
===========

Log rotation for Windows

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

The modes are determined automatically, depending on the settings of `Period` and `Size`. If Period is specified by a number, the first mode is used. In other cases, the second one is used (when Period is equal to Daily, Weekly or Monthly, or when the file size is specified). However, you can always set the mode you need manually by specifying `shift = no` for the first mode and `shift = yes` for the second.

Rotation settings are specified via ini configuration text files. (You can also use the format in java-style).
The default configuration file is mrotate.ini.

The general scheme of use is as follows: specify the settings for the rotations in the mrotate.ini file and add the daily launch of mrotate.exe / r to the scheduler

## Startup Options

* `/Help, /h` - Display the list of available keys
* `/Rotate, /r` - Run the rotation process for all loaded rotation settings
* `/Verify, /v` - Check rotation settings
* `/Debug, /d` - Emulation work, the files are not processed. You will see what happens if you start the program with the parameter `/r`
* `/Force, /f` - Force rotation, only for entries with `shift = yes`
* `/Conf = file, /c = file` - Download the specified file with the rotation settings, the parameter can be specified several times. If the parameter is specified, the file mrotate.ini does not load
* `/Arh = file, /a = file` - Load the settings of archiving rules from the specified file, the parameter can be specified several times. In addition to the above, the archivers.ini file is always processed
* `/Status = file, /s = file` - Use the specified rotation status file instead of the default

## Configuration File Format

The configuration file contains the rotation settings (the usual ini format). UTF-8 encoding


Common file format:

```ini
[Recording_Name1]
Options
[Recording_name2]
Options
```

If you change the name of the entry, you may lose information about the date of the last rotation and an extraordinary rotation will occur. The same when changing the name of the settings file. (Only with shift mode)

### Description of parameters

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
 To compress files specified by << arh, the archiving rule >> or not to compress ("no") - there will be a simple renaming
* `TargetDir`  
 The directory for placing old files, if not specified, the archive files are created near to the originals