# Nanolib
This is the Python Windows version of the Nanolib.

## Prerequisites Windows
1. A python 3 installation is required, we recommend the official version from [python.org](https://www.python.org/downloads/windows/).
2. We recommend using a virtual environment before installing nanoteclib, open a CMD and use the following commands to setup a virtual environment:
   ```cmd
   mkdir test_project
   cd test_project
   python -m venv .env
   .env\Scripts\activate.bat
   ```
   In case the setup was successful the CMD is prefixed with `(.env)`, e.g. `(.env) C:\test_project>`
3. The package 'wheel' is necessary to install nanoteclib:
   ```cmd
   pip install wheel
   ```

## Prerequisites Linux
1. A python 3 installation is required.
2. We highly recommend using pip and "virtual environment". Install both with the following bash command:
   ```bash
   sudo apt install python3-pip python3-venv -y
   ```
3. We recommend using a virtual environment before installing nanoteclib, use the following commands to setup a virtual environment:
   ```bash
   mkdir test_project
   cd test_project
   python3 -m venv .env
   source ./.env/bin/activate
   ```
   In case the setup was successful the bash is prefixed with `(.env)`, e.g. `(.env) username@hostname:~/test_project$ `
4. The package 'wheel' and 'setuptools' is necessary to install nanoteclib:
   ```bash
   pip3 install wheel
   pip3 install setuptools
   ```
5. The library requires access to the serial port. To grant access, add the User to the dialout group
   ```bash
   sudo adduser $USER dialout
   ```

## Installation
To install Nanolib use pip with the following bash script:
- Linux:
  ```bash
  pip3 install PATH_TO_NANOTEC_LIB_ZIP/nanoteclib-N.N.N.zip
  ```
- Windows
  ```cmd
  pip install PATH_TO_NANOTEC_LIB_ZIP/nanoteclib-N.N.N.zip
  ```

where `N` is the actual number of the nanoteclib.