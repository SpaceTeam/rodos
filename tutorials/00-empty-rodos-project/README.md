## Table of Contents
- [Supported Architectures](#supported-architectures)
- [STM32Cube IDE](#stm32cube-ide)
- [IDE installation and RODOS configuration](#ide-installation-and-rodos-configuration)
    - [on Ubuntu](#on-ubuntu)
    - [on Windows 10](#on-windows-10)
- [Start with STM32F4 Discavery Boards](#start-with-stm32f4-discavery-boards)
    - [STM32F407VG](#stm32f407vg)
    - [STM32F411VE](#stm32f411ve)
- [Start with linux-x86](#start-with-linux-x86)
    - [On Ubuntu](#on-ubuntu)
    - [on Windows](on-windows)
- [FAQs](#faqs)

## Unterstützte Architekturen
| Architecture | Windows 10 | Ubuntu 20.04 LTS | macOS |
|:--------------|:-------------:|:--------------:|:--------------:|
| STM32F407VG | **+** | **+** | **-** |
| STM32F411VE | **-** | **-** | **-** |
| linux-x86 | **+** | **+** | **-** |
***
NOTE: Not yet tested on macOS. But the IDE is also supported by macOS. Please see these [installation instruction](https://www.st.com/resource/en/user_manual/dm00603964-stm32cubeide-installation-guide-stmicroelectronics.pdf).

## STM32Cube IDE
[STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html#overview) is a multi-OS IDE based on [Eclipse IDE for C/C++ Developers](https://www.eclipse.org/downloads/packages/release/2021-06/r/eclipse-ide-cc-developers). It is already equipped with all the necessary plugins for programming the microcontrollers of the STM32 series.

## IDE installation and RODOS configuration
Download the IDE for free  [here](https://www.st.com/en/development-tools/stm32cubeide.html#overview) (registration required).

NOTE: For more, see these [installation instruction](https://www.st.com/resource/en/user_manual/dm00603964-stm32cubeide-installation-guide-stmicroelectronics.pdf).

### on Ubuntu
```
$ sudo apt update
$ sudo apt upgrade -y

$ sudo apt install zip build-essential gcc-multilib

$ cd /home/${USER}/Downloads/
# alternatively
# cd /path/to/downloaded_ide_zip_archive/

$ unzip -q en.st-stm32cubeide_*.deb_bundle.sh.zip
$ chmod 774 st-stm32cubeide_*.deb_bundle.sh
$ ./st-stm32cubeide_*.deb_bundle.sh
```
After the installation you will find the IDE in the _/opt/ST/_ directory. The start icon of STM32CubeIDE can be found under other desktop applications.

### On Windows 10
Extract the downloaded zip archive "__en.st-stm32cubeide_***_x86_64.exe.zip__" and run the EXE file "__st-stm32cubeide_***_x86_64.exe__.

#### Install Git plugin
After starting the IDE for the first time, check if **git** is installed. If not, install it from the Eclipse **Marketplace**.
> 1.    Click Help -> Eclipse Marketplace...
> 2.    Type "Egit" and and search for "EGit -Git Integration for Eclipse"
> 3.    Click on Install button

#### Clone RODOS project
> 1.    Copy this link __https://gitlab.com/rodos/rodos__
> 2.    Window -> Perspective -> Open Perspective -> Other...
> 3.    Double click on Git (Git perspective opens up)
> 4.    Click in Git Repositories View on Clone a Git Repository and add the clone to this view
> 5.    Next>
> 6.    Click on Deselect All and just choose the **eclipse-project** branch
> 7.    Next>
> 8.    Paste the path to the STM32Cube IDE workspace in the field Directory:
> 9.    Finish
> 10.    Window -> Perspective -> Open Perspective -> Other...
> 11.    Double click on C/C++ (default)
> 12.    File -> Import...
> 13.    Double click on General -> Existing Project into Workspace
> 14.    Paste the path to the STM32Cube IDE workspace in the field Select root directory:
> 15.    Select two projects HelloWorld and rodos in the field Projects:
> 16.    Finish

After cloning and importing, you will see two projects in the Project Explorer. Both projects have errors. The errors do not affect the build process. To clean up the errors, click Problems View, right-click on Errors (x items) and click Delete.

#### IDE Indexer configuration
You need this to see only the current platform build configurations.
> 1.    Window -> Preferences -> C/C++ -> Indexer
> 2.    Choose **Use active build configuration**
> 3.    Apply and Close

## Start with STM32F4 Discavery Boards

### STM32F407VG
> 1.    Right klick on rodos project
> 2.    Build Configurations -> Set Active -> Debug_STM32F407VG
> 3.    Right klick on rodos project
> 4.    Build Project
> 5.    Open rodos -> tutorials -> 00-empty-rodos-project -> HelloWorld
> 6.    Right klick on HelloWorld project
> 7.    Build Configurations -> Set Active -> Debug_STM32F407VG
> 8.    Right klick on HelloWorld project
> 9.    Build Project
> 10.    Click on Debug as (the green beetle)
> 11.    If the first time you run it, double-click on STM32 Cortex-M C/C++ Application

NOTE: When debbuging/flashing, make sure the HelloWorld (or your project) project is selected.

### STM32F411VE
In Progress

## Start with linux-x86

#### On Ubuntu
Build and run the Project
> 1.    Right klick on rodos project
> 2.    Build Configurations -> Set Active -> Debug_linux_x86_on_linux
> 3.    Right klick on rodos project
> 4.    Build Project
> 5.    Open rodos -> tutorials -> 00-empty-rodos-project -> HelloWorld
> 6.    Right klick on HelloWorld project
> 7.    Build Configurations -> Set Active -> Debug_linux_x86_on_linux
> 8.    Right klick on HelloWorld project
> 9.    Build Project
> 10.    Click on Run as (the green button)

#### On Windows
You need a cross compiler to compile Linux binary on Windows 10. To do this, first install [Cygwin](https://www.cygwin.com/). During the installation of Cygwin, install **make** (last version). After the installation add the path to the Cygwin64 root directory to the system environment variables. Download the toolchain from [here](https://mega.nz/file/G1hkiIZQ#NKup6JNL5xlC-G2K4aNizzniBbr0ToXjLIp8biyg3H8) and copy it to the Cygwin64 root directory. The toolchain signatures are 

CRC32: A47ACD7F
MD5: D04CE11D701BADC5473257376E9EC78E
SHA-1: 8E95FF3A74A65F03D7ADD571B59ED2EADCEBC52E

Also copy the script from RODOS Project Folder (/rodos/tutorials/00-empty-rodos-project/scripts/toolchain-install.sh) to the Cygwin64 root directory. Start Cygwin64 and install the toolchain.
```
$ cd /
$ chmod 774 toolchain-install.sh
$ ./toolchain-install.sh
```
NOTE: The Toolchain installation can take a few minutes.

Build the Project
> 1.    Right klick on rodos project
> 2.    Build Configurations -> Set Active -> Debug_linux_x86_on_Win
> 3.    Right klick on rodos project
> 4.    Build Project
> 5.    Open rodos -> tutorials -> 00-empty-rodos-project -> HelloWorld
> 6.    Right klick on HelloWorld project
> 7.    Build Configurations -> Set Active -> Debug_linux_x86_on_Win
> 8.    Right klick on HelloWorld project
> 9.    Build Projects

You cannot run a Linux x86 binary file on Windows. But you can do it in the [WSL (Windows Subsystem for Linux) version 2 ](https://docs.microsoft.com/en-us/windows/wsl/about). How do you install wsl can be found [here](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

NOTE: You need wsl version 2. Tested with WSL Ubuntu 20.04.

Configure wsl to run Linux x86 binaries:
Open Ubunut 20.04 (WSL)
```
$ sudo apt update
$ sudo apt upgrade -y

$ sudo dpkg --add-architecture i386
$ sudo apt-get update
$ sudo apt install gcc-multilib
```

Run Linux x86 binary file on Windows:
Open Ubunut 20.04 (WSL)
```
$ cd /mnt/c/PATH/TO/STM32CUBDEIDE_WORKSPACE/rodos/tutorials/00-empty-rodos-project/HelloWorld/Debug_linux_x86_on_Win/
$ ./HelloWorld
```

## FAQs
How can I create my own new RODOS project?
    - To create your own RODOS project, simply copy the HelloWorld project and paste it into the project explorer.

