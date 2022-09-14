using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace NanolibCsharpExampleWin
{
    /// <summary>
    /// Callback class derived from Nlc.NlcScanBusCallback.
    /// </summary>
    internal class ScanBusCallback : Nlc.NlcScanBusCallback // override
    {
        /// <summary>
        /// Callback used in bus scanning.
        /// </summary>
        /// <param name="info">Scan process state</param>
        /// <param name="devicesFound">Devices found so far</param>
        /// <param name="data">Optional data, meaning depends on info.</param>
        /// <returns></returns>
        public override Nlc.ResultVoid callback(Nlc.BusScanInfo info, Nlc.DeviceIdVector devicesFound, int data)
        {
            switch (info)
            {
                case Nlc.BusScanInfo.Start:
                    Console.WriteLine("Scan started.");
                    break;

                case Nlc.BusScanInfo.Progress:
                    if ((data & 1) == 0) // data holds scan progress
                    {
                        Console.Write(".");
                    }
                    break;

                case Nlc.BusScanInfo.Finished:
                    Console.WriteLine("");
                    Console.WriteLine("Scan finished.");
                    break;
            }

            return new Nlc.ResultVoid();
        }
    }

    internal class NanolibException : Exception
    {
        public NanolibException(string message) : base(message)
        {
        }

        public NanolibException(string message, Exception inner) : base(message, inner)
        {
        }

        public NanolibException()
        {
        }
    }

    /// <summary>
    /// Helper class used just to wrap around Nanolib.
    /// Of course, Nanolib can be used directly in the code.
    /// </summary>
    internal class NanolibHelper
    {
        private Nlc.NanoLibAccessor nanolibAccessor;

        /// <summary>
        /// Creates and stores the nanolib accessor
        /// Note: call this function before calling another function
        /// </summary>
        public void Setup()
        {
            // before accessing the nanolib, the pointer to the accessor class
            // needs to be created and stored somewhere
            nanolibAccessor = Nlc.Nanolib.getNanoLibAccessor();
        }

        /// <summary>
        /// Get the hardware objects from given accessor
        /// </summary>
        /// <returns>Array of HardwareIds</returns>
        public Nlc.BusHWIdVector GetBusHardware()
        {
            Nlc.ResultBusHwIds result = nanolibAccessor.listAvailableBusHardware();

            if (result.hasError())
            {
                string errorMessage = String.Format("Error: listAvailableBusHardware() - {0}", result.getError());
                throw new NanolibException(errorMessage);
            }
            return result.getResult();
        }

        /// <summary>
        /// Create bus hardware options object from given bus hardware id
        /// </summary>
        /// <param name="busHwId">The id of the bus hardware taken from GetHardware()</param>
        /// <returns>A set of options for opening the bus hardware</returns>
        public Nlc.BusHardwareOptions CreateBusHardwareOptions(Nlc.BusHardwareId busHwId)
        {
            // create new bus hardware options
            Nlc.BusHardwareOptions busHwOptions = new Nlc.BusHardwareOptions();

            // now add all options necessary for opening the bus hardware
            // in case of CAN bus it is the baud rate
            Nlc.BusHwOptionsDefault busHwOptionsDefaults = new Nlc.BusHwOptionsDefault();

            // now add all options necessary for opening the bus hardware
            if (busHwId.getProtocol() == Nlc.Nanolib.BUS_HARDWARE_ID_PROTOCOL_CANOPEN)
            {
                // in case of CAN bus it is the baud rate
                busHwOptions.addOption(busHwOptionsDefaults.canBus.BAUD_RATE_OPTIONS_NAME,
                                       busHwOptionsDefaults.canBus.baudRate.BAUD_RATE_1000K);

                if (busHwId.getBusHardware() == Nlc.Nanolib.BUS_HARDWARE_ID_IXXAT)
                {
                    // in case of HMS IXXAT we need also bus number
                    busHwOptions.addOption(
                        busHwOptionsDefaults.canBus.ixxat.ADAPTER_BUS_NUMBER_OPTIONS_NAME,
                        busHwOptionsDefaults.canBus.ixxat.adapterBusNumber.BUS_NUMBER_0_DEFAULT);
                }
            }
            else if (busHwId.getProtocol() == Nlc.Nanolib.BUS_HARDWARE_ID_PROTOCOL_MODBUS_RTU)
            {
                // in case of Modbus RTU it is the serial baud rate
                busHwOptions.addOption(busHwOptionsDefaults.serial.BAUD_RATE_OPTIONS_NAME,
                                       busHwOptionsDefaults.serial.baudRate.BAUD_RATE_19200);
                // and serial parity
                busHwOptions.addOption(busHwOptionsDefaults.serial.PARITY_OPTIONS_NAME,
                                       busHwOptionsDefaults.serial.parity.EVEN);
            }
            else
            {

            }

            return busHwOptions;
        }

        /// <summary>
        /// Opens the bus hardware with given id and options
        /// </summary>
        /// <param name="busHwId">The id of the bus hardware taken from GetHardware()</param>
        /// <param name="busHwOptions">The hardware options taken from Create.....BusHardwareOptions()</param>
        public void OpenBusHardware(Nlc.BusHardwareId busHwId, Nlc.BusHardwareOptions busHwOptions)
        {
            Nlc.Result result = nanolibAccessor.openBusHardwareWithProtocol(busHwId, busHwOptions);

            if (result.hasError())
            {
                string errorMsg = String.Format("Error: openBusHardwareWithProtocol() - {0}", result.getError());
                throw new NanolibException(errorMsg);
            }
        }

        /// <summary>
        /// Closes the bus hardware (access no longer possible after that)
        /// Note: the call of the function is optional because the nanolib will cleanup the
        /// bus hardware itself on closing.
        /// </summary>
        /// <param name="busHwId">The bus hardware id to close</param>
        public void CloseBusHardware(Nlc.BusHardwareId busHwId)
        {
            Nlc.Result result = nanolibAccessor.closeBusHardware(busHwId);

            if (result.hasError())
            {
                string errorMsg = String.Format("Error: closeBusHardware() - {0}", result.getError());
                throw new NanolibException(errorMsg);
            }
        }

        /// <summary>
        /// Scans bus and returns all found device ids.
        ///
        /// CAUTION: open bus hardware first with NanoLibHelper.OpenBusHardware()
        ///
        /// </summary>
        /// <param name="busHwId">The bus hardware to scan</param>
        /// <returns>Vector with found devices</returns>
        public Nlc.DeviceIdVector ScanBus(Nlc.BusHardwareId busHwId)
        {
            ScanBusCallback scanCallback = new ScanBusCallback();

            Nlc.ResultDeviceIds result = nanolibAccessor.scanDevices(busHwId, scanCallback);

            if (result.hasError())
            {
                string errorMsg = String.Format("Error: scanDevices() - {0}", result.getError());
                throw new NanolibException(errorMsg);
            }

            return result.getResult();
        }

        /// <summary>
        /// Create a Device and return DeviceHandle
        /// </summary>
        /// <param name="deviceId">The device id</param>
        /// <returns>The DeviceHandle used to access all device related functions</returns>
        public Nlc.DeviceHandle CreateDevice(Nlc.DeviceId deviceId)
        {
            Nlc.ResultDeviceHandle result = nanolibAccessor.addDevice(deviceId);

            if (result.hasError())
            {
                string errorMsg = String.Format("Error: CreateDevice() - {0}", result.getError());
                throw new NanolibException(errorMsg);
            }

            return result.getResult();
        }

        /// <summary>
        /// Connects to given DeviceHandle
        /// </summary>
        /// <param name="deviceHandle"></param>
        public void ConnectDevice(Nlc.DeviceHandle deviceHandle)
        {
            Nlc.ResultVoid result = nanolibAccessor.connectDevice(deviceHandle);

            if (result.hasError())
            {
                throw new NanolibException("Error: ConnectDevice() - " + result.getError());
            }
        }

        /// <summary>
        /// Disconnects given device
        ///
        /// Note: the call of the function is optional because the nanolib will cleanup the
        /// devices on bus itself on closing.
        /// </summary>
        /// <param name="deviceHandle">DeviceHandle of the device</param>
        public void DisconnectDevice(Nlc.DeviceHandle deviceHandle)
        {
            Nlc.ResultVoid result = nanolibAccessor.disconnectDevice(deviceHandle);

            if (result.hasError())
            {
                throw new NanolibException("Error: DisconnectDevice() - " + result.getError());
            }
        }

        /// <summary>
        /// Reads out a number of given device
        /// </summary>
        /// <param name="deviceHandle">The handle of the device to read from</param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to read from</param>
        /// <returns>A 64 bit number. The interpretation of the data type is up to the user. </returns>
        public long ReadNumber(Nlc.DeviceHandle deviceHandle, Nlc.OdIndex odIndex)
        {
            Nlc.ResultInt result = nanolibAccessor.readNumber(deviceHandle, odIndex);

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Reading number", deviceHandle, odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
            return result.getResult();
        }

        /// <summary>
        /// Reads out a number of given device via the assigned object dictionary
        /// </summary>
        /// <param name="objectDictionay">An assigned object dictionary</param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to read from</param>
        /// <returns>A 64 bit number. The interpretation of the data type is up to the user. </returns>
        public long ReadNumber(Nlc.ObjectDictionary objectDictionay, Nlc.OdIndex odIndex)
        {
            Nlc.ResultInt result = GetObject(objectDictionay, odIndex).readNumber();

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Reading number", objectDictionay.getDeviceHandle().getResult(),
                    odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
            return result.getResult();
        }

        /// <summary>
        /// Writes given number to the device
        /// </summary>
        /// <param name="deviceHandle">The id of the device to write to</param>
        /// <param name="value">The value to write to the device</param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to write to</param>
        /// <param name="bitLength">The bit length of the object to write to, either 8, 16 or 32 (see manual for all the bit lengths of all objects)</param>
        public void WriteNumber(Nlc.DeviceHandle deviceHandle, long value, Nlc.OdIndex odIndex, uint bitLength)
        {
            Nlc.ResultVoid result = nanolibAccessor.writeNumber(deviceHandle, value, odIndex, bitLength);

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Writing number", deviceHandle, odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
        }

        /// <summary>
        /// Writes given number to the device via assigned object dictionary
        /// </summary>
        /// <param name="objectDictionay">An assigned object dictionary</param>
        /// <param name="value">The value to write to the device</param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to write to</param>
        public void WriteNumber(Nlc.ObjectDictionary objectDictionay, long value, Nlc.OdIndex odIndex)
        {
            Nlc.ResultVoid result = GetObject(objectDictionay, odIndex).writeNumber(value);

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Writing number", objectDictionay.getDeviceHandle().getResult(), odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
        }

        /// <summary>
        /// Reads array object from a device
        /// </summary>
        /// <param name="deviceHandle">The handle of the device to read from</param>
        /// <param name="index">The index of the object</param>
        /// <returns>Vector (array) of numbers (the interpretation of the data type is up to the user).</returns>
        public Nlc.IntVector ReadArray(Nlc.DeviceHandle deviceHandle, ushort index)
        {
            var result = nanolibAccessor.readNumberArray(deviceHandle, index);

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Reading array", deviceHandle, new Nlc.OdIndex(index, 0), result.getError());
                throw new NanolibException(errorMsg);
            }
            return result.getResult();
        }

        /// <summary>
        /// Reads a string object from a devicew
        /// </summary>
        /// <param name="deviceHandle"></param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to read from</param>
        /// <returns>string</returns>
        public string ReadString(Nlc.DeviceHandle deviceHandle, Nlc.OdIndex odIndex)
        {
            Nlc.ResultString result = nanolibAccessor.readString(deviceHandle, odIndex);            

            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Reading string", deviceHandle, odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
            return result.getResult();
        }

        /// <summary>
        /// Reads a string object from a given device via the assigned object dictionary
        /// </summary>
        /// <param name="deviceHandle"></param>
        /// <param name="odIndex">The index and sub-index of the object dictionary to read from</param>
        /// <returns>string</returns>
        public string ReadString(Nlc.ObjectDictionary objectDictionay, Nlc.OdIndex odIndex)
        {
            Nlc.ResultString result = GetObject(objectDictionay, odIndex).readString();
            if (result.hasError())
            {
                string errorMsg = CreateErrorMessage("Reading string", objectDictionay.getDeviceHandle().getResult(),
                    odIndex, result.getError());
                throw new NanolibException(errorMsg);
            }
            return result.getResult();
        }

        /// <summary>
        /// Gets DeviceId of given DeviceHandle.
        /// </summary>
        /// <param name="deviceHandle">Nlc.DeviceHandle</param>
        /// <returns>Nlc.DeviceId</returns>
        public Nlc.DeviceId GetDeviceId(Nlc.DeviceHandle deviceHandle)
        {
            Nlc.ResultDeviceId result = nanolibAccessor.getDeviceId(deviceHandle);
            if (result.hasError())
            {
                throw new NanolibException("Unable to get DeviceId");
            }

            return result.getResult();
        }

        /// <summary>
        /// Gets assigned object dictionary.
        /// </summary>
        /// <param name="deviceHandle">Nlc.DeviceHandle.</param>
        /// <returns>Assigned Nlc.ObjectDictionary.</returns>
        public Nlc.ObjectDictionary GetDeviceObjectDictionary(Nlc.DeviceHandle deviceHandle)
        {
            Nlc.ResultObjectDictionary result = nanolibAccessor.getAssignedObjectDictionary(deviceHandle);
            if (result.hasError())
            {
                throw new NanolibException("Unable to get the assigned Object Dictionary");
            }

            return result.getResult();
        }

        /// <summary>
        /// Gets object dictionary entry.
        /// </summary>
        /// <param name="objectDictionary">Nlc.ObjectDictionary.</param>
        /// <param name="odIndex">Nlc.OdIndex.</param>
        /// <returns>Nlc.ObjectEntry.</returns>
        public Nlc.ObjectEntry GetObjectEntry(Nlc.ObjectDictionary objectDictionary, ushort index)
        {
            Nlc.ResultObjectEntry result = objectDictionary.getObjectEntry(index);
            if (result.hasError())
            {
                throw new NanolibException("Unable to get Object Dictionary entry");
            }

            return result.getResult();
        }

        /// <summary>
        /// Gets object dictionary sub entry.
        /// </summary>
        /// <param name="objectDictionary">Nlc.ObjectDictionary.</param>
        /// <param name="odIndex">Nlc.OdIndex.</param>
        /// <returns>Nlc.ObjectSubEntry.</returns>
        public Nlc.ObjectSubEntry GetObject(Nlc.ObjectDictionary objectDictionary, Nlc.OdIndex odIndex)
        {
            Nlc.ResultObjectSubEntry result = objectDictionary.getObject(odIndex);
            if (result.hasError())
            {
                throw new NanolibException("Unable to get Object Dictionary sub entry");
            }

            return result.getResult();
        }

        /// <summary>
        /// Gets Sampler Interface.
        /// </summary>
        /// <returns>Nlc.SamplerInterface</returns>
        public Nlc.SamplerInterface GetSamplerInterface()
        {
            return nanolibAccessor.getSamplerInterface();
        }

        /// <summary>
        /// Gets ProfinetDCP Interface.
        /// </summary>
        /// <returns>Nlc.SamplerInterface</returns>
        public Nlc.ProfinetDCP GetProfinetDcpInterface()
        {
            return nanolibAccessor.getProfinetDCP();
        }

        /// <summary>
        /// Set the logging level
        /// </summary>
        /// <param name="logLevel">Nanolib log Level</param>
        public void SetLoggingLevel(Nlc.LogLevel logLevel)
        {
            if (nanolibAccessor == null)
            {
                throw new NanolibException("Error: NanolibHelper2021().setup() is required");
            }
            nanolibAccessor.setLoggingLevel(logLevel);
        }

        /// <summary>
        /// Helper function for creating an error message from given objects
        /// </summary>
        /// <param name="function">The name of the function the error ocurred</param>
        /// <param name="deviceHandle">The handle of the device to read from</param>
        /// <param name="odIndex">The index and sub-index of the object dictionary</param>
        /// <param name="resultError">The error text of the result</param>
        /// <returns></returns>
        private string CreateErrorMessage(string function, Nlc.DeviceHandle deviceHandle, Nlc.OdIndex odIndex, string resultError)
        {
            string deviceIdstring;
            Nlc.ResultDeviceId resultDeviceId = nanolibAccessor.getDeviceId(deviceHandle);
            if (resultDeviceId.hasError())
            {
                deviceIdstring = "invalid handle";
            }
            else
            {
                deviceIdstring = resultDeviceId.getResult().toString();
            }

            return String.Format("{0} of device {1} at od index {2} resulted in an error: {3}", function, deviceIdstring, odIndex.Index.ToString(), resultError);
        }
    }
}