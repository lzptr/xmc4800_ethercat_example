# xmc4800_ethercat_example

This is a minimal EtherCAT Slave based around the XMC4800 Relax Kit from Infineon and the open source 
EtherCAT Slave Stack SOES (simple open ethercat slave).

Designed and tested on Windows with WSL2 & linux.

## Contents:
* [1. Dependencies](#1-dependencies)
* [2. Getting Started](#2-getting-started)
	* [2.1 Build Using the Command Line](#21-build-using-the-command-line)
	* [2.2 Build Using VS Code](#22-build-using-vs-code)
	* [2.3 Debugging in WSL2 with USB Passthrough](#23-debugging-in-wsl2-with-usb-passthrough)
		* [2.3.1 Setup USB Passthrough](#231-setup-usb-passthrough)

## 1) Dependencies

 - VSCode with Cortex-Debug Extension 
 - cmake 
 - make
 - arm-none-eabi-gcc - You can use the bootstrap.sh script from libs/platforms/scripts to automatically install it in /opt/toolchain
 - JLink tools - You can use the bootstrap.sh script from libs/platforms/scripts to automatically install it in /opt/toolchain
 - If you want to use the Docker based devcontainer environment, then you also need to have docker installed
---

## 2) Getting Started

Download the repository and it's submodules.

To get the cross compiler and the debugging tools you can execute the bootstrap script:

	$ sudo chmod +x libs/platforms/scripts/bootstrap.sh
	$ ./libs/platforms/scripts/bootstrap.sh --gcc --jlink

The script downloads the gcc-arm-none-eabi compiler and Segger JLink
and installs them in /opt/toolchain.

The bootstrap script also copies the udev rules for JLink.


### 2.1) Build Using the Command Line

	$ mkdir build
	$ cd build
	$ cmake -DCMAKE_BUILD_TYPE=Debug ..
	$ make

### 2.2) Build Using VS Code

There are a number of the vs code tasks that are already predefined.
Use the BuildDebug task to build the project using cmake and make.

---

### 2.3) Debugging in WSL2 with USB Passthrough

The newest WSL2 kernel version includes USBIP based USB passthrough.
This means, we can debug directly out of WSL without any Windows tools.


#### 2.3.1) Setup USB Passthrough

First follow the steps here to install the usb passthrough support:
https://devblogs.microsoft.com/commandline/connecting-usb-devices-to-wsl/

Make sure to install the usbipd-win server mentioned at the start of the link above.

Debugging using a Segger J-Link device requires the JLink tools to be installed.
If you executed the script you're good to go.
Otherwise download and install from https://www.segger.com/jlink-software.html for linux.


Now you need to forward the USB connection from windows to WSL2.
Open a powershell or bash with administrative rights.
WSL2 can also call windows executables, so the only thing that counts is that you have opened the terminal with windows administrative rights.

Next plugin the board and list all USB devices:

	usbipd.exe wsl list

Make sure that you see the J-Link driver, otherwise the debugger connection could break when the target is reseted at 
the beginning of the session. You should see something like this:

	BUSID  DEVICE                                                        STATE
	1-11   1366:0105  JLink CDC UART Port (COM5), J-Link driver          Not attached

Now attach it to WSL2 with the auto attach option and check again:

	usbipd.exe wsl attach --busid 1-11 -a
	usbipd.exe wsl list

	BUSID  DEVICE                                                        STATE
	1-11   1366:0105  JLink CDC UART Port (COM5), J-Link driver          Attached - Ubuntu-20.04

Sometimes when restarting the PC, then the udev service is not running in WSL2.
This means that we can't connect using JLink because the udev rules aren't active.
To automate the restart process, I created a windows batch file with the following content:

	wsl -d Ubuntu-20.04 echo ""
	wsl -d Ubuntu-20.04 -u root service udev restart
	wsl -d Ubuntu-20.04 -u root udevadm control --reload
	exit

Then you need to create a windows task that get's run when you log in.
Here is an example for that: https://medium.com/swlh/how-to-run-ubuntu-in-wsl2-at-startup-on-windows-10-c4567d6c48f1

This basically restarts the udev service and reloads all udev rules when you log in into your account.

If you don't use the window batch script, you need to take care of the order of the steps.
If the the usb device is already attached to WSL2, then restarting udev and reloading doesn't seem to have an effect.
First detach the device, then restart udev and then reattach.

#### 2.3.2) Debug with VS Code

Now go to VS Code, where you build the debug version of the example firmware and just hit F5.
If you used the bootstrap script then it should just work.
Otherwise, go to the .vscode/launch.json file and adapt the paths to the tools to your specific setup.
If you are not using the script, you will probably need to set these 2 properties:

	"serverpath": "<path_to_local_jlink_binaries>"
	"armToolchainPath": "<path to your arm_none_eabi_gcc binary folder>"

The cortex-debug extension can show the actual register values of chip while debugging.
For that purpose, the launch file uses a svd file that is part of the xmc library.

If you use another chip, you need to add your own SVD file.
Here is a blog post where you can find other SVD files:
	https://community.silabs.com/s/article/svd-file-for-efm32-device?language=en_US

With all of this set in place, you can use the visual debugger of vs code to debug you 
microcontroller!
Build the firmware and hit "F5" to start the debugging sessions with the board attached and set some breakpoints.

