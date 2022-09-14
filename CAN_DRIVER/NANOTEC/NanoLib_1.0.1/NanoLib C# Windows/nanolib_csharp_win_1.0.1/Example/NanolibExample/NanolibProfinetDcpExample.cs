using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NanolibCsharpExampleWin
{
    /// <summary>
    /// An example of using the ProfinetDCP interface from NanoLib
    /// </summary>
    internal class NanolibProfinetDcpExample
    {
        private readonly Nlc.ProfinetDCP profinetDCP;

        /// <summary>
        /// Creates the ProfinetDCP example.
        /// </summary>
        /// <param name="helper">NanolibHelper</param>
        public NanolibProfinetDcpExample(NanolibHelper helper)
        {
            profinetDCP = helper.GetProfinetDcpInterface();
        }

        /// <summary>
        /// Checks whether we can use ProfinetDCP and scan the available devices.
        /// </summary>
        /// <param name="busHwId">Nlc.BusHardwareId</param>
        public void ProfinetDcpExample(Nlc.BusHardwareId busHwId)
        {
            // We can use ProfinetDCP only on "network" bus hardware.
            if (busHwId.getBusHardware().Equals(Nlc.Nanolib.BUS_HARDWARE_ID_NETWORK))
            {
                // Check whether we can use ProfinetDCP on this bus hardware (PCap driver is installed)
                Nlc.ResultVoid result = profinetDCP.isServiceAvailable(busHwId); 

                if (!result.hasError())
                {
                    Nlc.ResultProfinetDevices resultDevices = profinetDCP.scanProfinetDevices(busHwId);

                    if (!resultDevices.hasError())
                    {
                        Nlc.ProfinetDeviceVector devices = resultDevices.getResult();
                        if (devices.Count > 0)
                        {
                            int i = 0;
                            Console.WriteLine("Found Profinet devices:");
                            foreach (Nlc.ProfinetDevice device in devices)
                            {
                                string ipStr = String.Format("{0}.{1}.{2}.{3}",
                                    (device.ipAddress >> 24),
                                    (device.ipAddress >> 16) & 0xFF,
                                    (device.ipAddress >> 8) & 0xFF,
                                    device.ipAddress & 0xFF);
                                Console.WriteLine(String.Format("{0}. Name: {1}, IP: {2}", i, device.deviceName, ipStr));
                            }

                            Console.WriteLine("Blink the first found device.");
                            profinetDCP.blinkProfinetDevice(busHwId, devices.ToArray()[0]);
                        }
                        else
                        {
                            Console.WriteLine("No Profinet devices found.");
                        }
                    }
                }
            }
        }
    }
}
