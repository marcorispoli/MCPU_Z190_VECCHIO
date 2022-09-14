using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NanolibCsharpExampleWin
{
    internal class NanolibExample
    {
        private static void Main(string[] args)
        {
            NanolibHelper nanolibHelper = new NanolibHelper();


            try
            {
                nanolibHelper.Setup();

                // its possible to set the logging level to a different level
                nanolibHelper.SetLoggingLevel(Nlc.LogLevel.Off);

                // list all hardware available, decide for the first one
                Nlc.BusHWIdVector busHwIds = nanolibHelper.GetBusHardware();

                if (busHwIds.Count() <= 0)
                {
                    Console.Error.WriteLine("No bus hardware found");

                    //Wait before close console
                    PressAnyKey();
                    return;
                }

                Console.WriteLine();
                Console.WriteLine("Available bus hardware:");
                Console.WriteLine();

                uint lineNum = 0;

                // just for better overview: print out available hardware
                foreach (Nlc.BusHardwareId adapter in busHwIds)
                {
                    Console.Write("{0}. {1}", lineNum, adapter.getName());
                    Console.WriteLine(" with protocol: {0}", adapter.getProtocol());
                    lineNum++;
                }

                Console.WriteLine();

                if (lineNum > 1)
                {
                    Console.Write("Please select (enter) bus hardware number(0-" + (lineNum - 1) + ") and press [ENTER]:");

                    try
                    {
                        lineNum = uint.Parse(Console.ReadLine());
                    }
                    catch (FormatException)
                    {
                    }

                    Console.WriteLine();

                    if (lineNum >= busHwIds.Count)
                    {
                        Console.WriteLine("Invalid selection!");
                        return;
                    }
                }
                else
                {
                    lineNum = 0;
                }

                // let's select the first one
                //Nlc.BusHardwareId busHwId = busHwIds[(int)lineNum]; // Do not do this !!!!
                //Nlc.BusHardwareId busHwId = new Nlc.BusHardwareId(busHwIds[(int)lineNum]); // Very important step in the C# version of NanoLib

                // Create a copy of every object, which is returned in a vector, because
                // when the vector goes out of scope, the contained objects will be destroyed.
                // or just copy the vector into an array
                Nlc.BusHardwareId busHwId = busHwIds.ToArray()[lineNum];

                // create bus hardware options for opening the hardware
                Nlc.BusHardwareOptions busHwOptions = nanolibHelper.CreateBusHardwareOptions(busHwId);

                // now able to open the hardware itself
                nanolibHelper.OpenBusHardware(busHwId, busHwOptions);

                try
                {

                    Console.WriteLine("Scanning for devices...");

                    // either scan the whole bus for devices (in case the bus supports scanning)
                    Nlc.DeviceIdVector deviceIds = nanolibHelper.ScanBus(busHwId);

                    Console.WriteLine("");
                    foreach (Nlc.DeviceId id in deviceIds)
                    {
                        Console.WriteLine("Found device: {0}", id.getDeviceId());
                    }

                    if (deviceIds.Count == 0)
                    {
                        Console.WriteLine("No devices found.");
                        PressAnyKey();
                        return;
                    }

                    Console.WriteLine();
                    Console.WriteLine("Available devices:");
                    Console.WriteLine();

                    lineNum = 0;

                    // just for better overview: print out available devices
                    foreach (Nlc.DeviceId id in deviceIds)
                    {
                        Console.Write("{0}. {1}", lineNum, id.getDescription());
                        Console.WriteLine("[device id: {0}, hardware: {1}]", id.getDeviceId(), id.getBusHardwareId().getName());
                        lineNum++;
                    }

                    Console.WriteLine();

                    if (lineNum > 1)
                    {
                        Console.Write("Please select (enter) device number(0-" + (lineNum - 1) + ") and press [ENTER]:");
                        try
                        {
                            lineNum = uint.Parse(Console.ReadLine());
                        }
                        catch (FormatException)
                        {
                        }

                        Console.WriteLine();

                        if (lineNum >= deviceIds.Count)
                        {
                            Console.WriteLine("Invalid selection!");
                            return;
                        }
                    }
                    else
                    {
                        lineNum = 0;
                    }

                    // We can create the device id manually
                    // Nlc.DeviceId deviceId = new Nlc.DeviceId(busHwId, 1, "");
                    // or get first found device

                    // Nlc.DeviceId deviceId = deviceIds[(int)lineNum]; // Do not do this !!!
                    //Nlc.DeviceId deviceId = new Nlc.DeviceId(deviceIds[(int)lineNum]); // Very important step in the C# version of NanoLib

                    // Create a copy of every object, which is returned in a vector, because
                    // when the vector goes out of scope, the contained objects will be destroyed.
                    // or just copy the vector into an array
                    Nlc.DeviceId deviceId = deviceIds.ToArray()[lineNum];


                    // Create a device and get a handle
                    Nlc.DeviceHandle deviceHandle = nanolibHelper.CreateDevice(deviceId);

                    // now connect to the device
                    nanolibHelper.ConnectDevice(deviceHandle);

                    try
                    {

                        // now ready to work with the device, here are some examples on how to access the
                        // object dictionary:
                        ObjectDictionaryAccessExamples(nanolibHelper, deviceHandle);
                        ObjectDictionaryAccessExamplesViaDictionaryInterface(nanolibHelper, deviceHandle);

                        NanolibSamplerExample samplerExample = new NanolibSamplerExample(nanolibHelper, deviceHandle);

                        if (AskTheUser("Do you want to run sampler examples?"))
                        {
                            // Run sampler examples
                            samplerExample.RunSamplerExamples();
                        }

                        NanolibProfinetDcpExample nanolibProfinetDcpExample = new NanolibProfinetDcpExample(nanolibHelper);

                        Console.WriteLine();

                        // ProfinetDCP example
                        nanolibProfinetDcpExample.ProfinetDcpExample(busHwId);
                    }
                    finally
                    {
                        // cleanup and close everything
                        Console.WriteLine("Disconnecting the device.");
                        nanolibHelper.DisconnectDevice(deviceHandle);
                    }
                } 
                finally
                {
                    // close the hardware
                    Console.WriteLine("Closing the hardware bus.");
                    nanolibHelper.CloseBusHardware(busHwId);
                }

                Console.WriteLine("");

                // Wait before close console
                PressAnyKey();
            }
            catch (NanolibException e)
            {
                Console.Error.WriteLine("Error occured: {0}", e.Message);

                //Wait before close console
                PressAnyKey();
            }
        }

        private static void ObjectDictionaryAccessExamples(NanolibHelper nanolibHelper, Nlc.DeviceHandle deviceId)
        {
            // OD example
            Console.WriteLine("");
            Console.WriteLine("OD Example");
            Console.WriteLine("");

            // read status word
            Console.WriteLine("Reading subindex 0 of index 0x6040");
            long statusWord = nanolibHelper.ReadNumber(deviceId, new Nlc.OdIndex(0x6040, 0x00));
            Console.WriteLine("Result: {0}", statusWord);
            Console.WriteLine("");

            // write control word to value "6"
            Console.WriteLine("Motor Stop (0x6040-0)");
            Console.WriteLine("");
            nanolibHelper.WriteNumber(deviceId, 6, new Nlc.OdIndex(0x6040, 0x00), 16);

            // read homepage of nanotec
            Console.WriteLine("Read Nanotec home page string");
            Console.WriteLine("The home page of Nanotec Electronic GmbH & Co. KG is: {0}", nanolibHelper.ReadString(deviceId, new Nlc.OdIndex(0x6505, 0x00)));
            Console.WriteLine("");

            // read error stack
            Console.WriteLine("Read device error stack");
            Nlc.IntVector errorStack = nanolibHelper.ReadArray(deviceId, 0x1003);
            Console.WriteLine("The error stack has {0} elements", errorStack.ElementAt(0));
            Console.WriteLine("");
        }

        private static void ObjectDictionaryAccessExamplesViaDictionaryInterface(NanolibHelper nanolibHelper, Nlc.DeviceHandle deviceHandle)
        {
            Nlc.OdIndex homePageOd = new Nlc.OdIndex(0x6505, 0x00);
            Nlc.OdIndex controlWordOd = new Nlc.OdIndex(0x6040, 0x00);


            // OD example
            Console.WriteLine("");
            Console.WriteLine("OD Example via Dictionary Interface");
            Console.WriteLine("");

            Nlc.ObjectDictionary objectDictionary = nanolibHelper.GetDeviceObjectDictionary(deviceHandle);

            // read status word
            Console.WriteLine("Reading subindex 0 of index 0x6040");
            long statusWord = nanolibHelper.ReadNumber(objectDictionary, new Nlc.OdIndex(0x6040, 0x00));
            Console.WriteLine("Result: {0}", statusWord);
            Console.WriteLine("");

            // write control word to value "6"
            Console.WriteLine("Motor Stop (0x6040-0)");
            Console.WriteLine("");
            nanolibHelper.WriteNumber(objectDictionary, 6, new Nlc.OdIndex(0x6040, 0x00));

            // read homepage of nanotec
            Console.WriteLine("Read Nanotec home page string");
            Console.WriteLine("The home page of Nanotec Electronic GmbH & Co. KG is: {0}", nanolibHelper.ReadString(objectDictionary, homePageOd));
            Console.WriteLine("");

            Nlc.ObjectEntry objectEntry = nanolibHelper.GetObjectEntry(objectDictionary, controlWordOd.Index);
            Console.WriteLine("Some ObjectEntry properties:");
            Console.WriteLine("Object(0x{0:X04}).ObjectCode = {1}", controlWordOd.Index, objectEntry.getObjectCode().ToString());
            Console.WriteLine("Object(0x{0:X04}).DataType = {1}", controlWordOd.Index, objectEntry.getDataType().ToString());
            Console.WriteLine();

            Console.WriteLine("Some ObjectSubEntry properties:");
            Nlc.ObjectSubEntry objectSubEntry = nanolibHelper.GetObject(objectDictionary, controlWordOd);
            Console.WriteLine("OdIndex(0x{0:X04}:{1:X02}).DataType = {2}", controlWordOd.Index, controlWordOd.SubIndex, objectSubEntry.getDataType().ToString());
            Console.WriteLine("OdIndex(0x{0:X04}:{1:X02}).BitLength = {2}", controlWordOd.Index, controlWordOd.SubIndex, objectSubEntry.getBitLength().ToString());
            Console.WriteLine();

        }

        // Wait for any key so console window will not close immediately
        private static void PressAnyKey()
        {
            Console.WriteLine("Press any key...");
            Console.ReadKey();
        }

        private static bool AskTheUser(string question)
        {
            Console.Write(question);
            Console.Write(" Press 'Y'es / 'N'o: ");

            char key = Console.ReadKey().KeyChar;

            Console.WriteLine();

            return (key == 'Y') || (key == 'y');
        }
    }
}