# Nanolib

This is the C++ Version of the Nanolib.

## Windows
# Example
1. Open Visual Studio 2019, create a new console application project.
2. Select a configuration Release, 64-bit
3. Open the settings of the project
   1. In the compiler settings
      1. add the "include" path to the "Additional Include Paths"
      2. and set the "C++ Language Standard" (under "Language") to "ISO C++17 (/std:c++17)"
   2. In the linker settings
     1. add the path of the *.dll to "Additional Library Directories"
	 2. add the file "nanolib.lib" to the "Additional Dependencies"
4. Copy the following code to the main file:
```C++
#include "accessor_factory.hpp"
#include <iostream>

int main()
{
	// DO NOT TRY TO delete THIS POINTER!
	nlc::NanoLibAccessor* nanolibAccessor = getNanoLibAccessor();
	nlc::ResultBusHwIds result = nanolibAccessor->listAvailableBusHardware();

	if (result.hasError())
	{
		std::cerr << "listAvailableBusHardware failed with error: " <<
			static_cast<int>(result.getErrorCode()) << " " << result.getError() << std::endl;
	}
	else
	{
		auto busHardwareIds(result.getResult());

		std::cout << "Available hardware buses and protocols:" << std::endl;

		for (const auto& busHardwareId : busHardwareIds)
		{
			std::cout << busHardwareId.getName()
				<< " protocol: " << busHardwareId.getProtocol() << std::endl;
		}
	}

	return 0;
}
```
5. Compile everything
6. Before running the executable, copy the files "nanolib.dll" and all nanolibm_*.dll files to the same folder of the executable.
7. Upon successful execution, the application will display a list of available hardware buses and communication protocols

## Linux

1. Install the Library
```bash
  sudo make install
```
2. Build the example
```bash
  make
```
 Note: The following command from the make script will require sudo privileges:
>sudo setcap cap_net_raw,cap_net_admin=eip $(OUT_PATH)example

It is necessary to allow Modbus TCP bus scanning.
see: https://linux.die.net/man/7/capabilities

4. Execute the example
```bash
  ./bin/example
```

5. Uninstall the Library if needed
```bash
  sudo make uninstall
```
