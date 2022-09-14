--- Nanolib ---
This is the Nanotec C# Nuget-Repository. The NanoLib offers an easy to use library to control Nanotec devices
www.nanotec.de

-- Quick Start --

# Prerequisites

- Unpack all files and folders of the nanolib_csharp_win_###.zip file.
- Create a folder for your local repository - Example: C:\NugetRepo
- Copy the Nanolib.####.nupkg from the zip file in your local Repository folder
- Add the local repository to Visual Studio: Tools -> NuGet Package Manager -> Package Sources -> Add -> Add your directory

# Create new project

- Open Visual Studio, on the welcome screen click on "Create new project".
- You can now choose the type of the project. Choose "Console App (.NET Framework) - C#, click next.
- You now need to give the project a name (e.g. "NanolibTest") and set its location. Select Framework: .NET Framework 4.7.2 Click on "Create"
- Add Nanolib Nuget Package by Right-Click on your project -> Manage Nuget Packages... -> Browse -> Nanolib -> Choose the latest Version and click "Install"
- If you cannot see the Nanolib-Package your "Package source" might be not selected
- Set the Target Platform to x64 -> Right Click on your project -> Properties -> Build -> Platform target: x64

# Test and compile

- Open the main (Program.cs in this example) and replace the text with the following code:

class Program
{
        static void Main(string[] args)
        {
            Nlc.NanoLibAccessor accessor = Nlc.Nanolib.getNanoLibAccessor();
        }
 }

- Go to the menu "Build" and choose "Build solution"
- In the compile output window, there should be no error but something like:
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

