using NanolibCsharpExampleWin;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace NanolibCsharpExampleWin
{
    internal delegate void ProcessSampledData(Nlc.SampleDataVector sampleDatas);

    /// <summary>
    /// SamplerNotifyCallback class derived from Nlc.SamplerNotify.
    /// </summary>
    internal class SamplerNotifyCallback : Nlc.SamplerNotify
    {
        private readonly EventWaitHandle waitHandle = new EventWaitHandle(false, EventResetMode.AutoReset);
        readonly ProcessSampledData processSampleData;

        /// <summary>
        /// Creates sampler notification callback.
        /// </summary>
        /// <param name="processSampleData">Sampled data processing delegate.</param>
        public SamplerNotifyCallback(ProcessSampledData processSampleData) : base()
        {
            this.processSampleData = processSampleData;
        }

        /// <summary>
        /// Callback used for notification on sampler events
        /// </summary>
        /// <param name="lastError">Last sampler error</param>
        /// <param name="samplerState">Current sampler state</param>
        /// <param name="sampleDatas">New sampler data</param>
        /// <param name="applicationData">Application supplied data (see Sampler.start(..))</param>
        public override void notify(Nlc.ResultVoid lastError, Nlc.SamplerState samplerState, Nlc.SampleDataVector sampleDatas, long applicationData)
        {
            // Be aware that notifications are executed in the context of separate threads 
            // other than thread that started the sampler.
            // 
            // Be careful when calling Nanolib functionality here, as doing so may cause this method
            // to be called recursively, potentially causing your application to deadlock.
            // 
            // For the same reason, this method should not throw exceptions.

            processSampleData(sampleDatas);

            if ((samplerState != Nlc.SamplerState.Ready) && (samplerState != Nlc.SamplerState.Running))
            {
                if (samplerState == Nlc.SamplerState.Failed)
                {
                    Console.Error.WriteLine("Sampler execution failed with error: {0}", lastError.getError());
                }

                // It's now safe to destroy this notification object
                waitHandle.Set();
            }
        }

        /// <summary>
        /// Wait for the sampler to finish.
        /// </summary>
        public void Wait()
        {
            waitHandle.WaitOne();
        }

        /// <summary>
        /// Wait for the sampler to finish.
        /// </summary>
        /// <param name="timeOut">Timeout value.</param>
        /// <returns>True if sampler has finished, false on timeout.</returns>
        public bool Wait(int timeOut)
        {
            return waitHandle.WaitOne(timeOut);
        }
    }

    /// <summary>
    /// Examples of using NanoLib sampler.
    /// </summary>
    internal class NanolibSamplerExample
    {
        private readonly Nlc.SamplerInterface sampler;
        private readonly NanolibHelper helper;
        private readonly Nlc.DeviceHandle deviceHandle;
        private readonly Nlc.OdIndex triggerAddress = new Nlc.OdIndex(0x2400, 1); // We just use first NanoJ input for the example
        private readonly Nlc.OdIndexVector trackedAddresses = new Nlc.OdIndexVector(
                new Nlc.OdIndex[] {
                    new Nlc.OdIndex(0x230F, 0), // Up time in seconds
                    new Nlc.OdIndex(0x4014, 3) // Temperature
                });
        private readonly string[] addressNames = new string[] { "Up time", "Temperature" };

        private readonly Nlc.SamplerConfiguration configuration = new Nlc.SamplerConfiguration();

        private bool headerPrinted;

        ulong sampleNumber;
        ulong lastIteration = ulong.MaxValue;

        /// <summary>
        /// Creates sampler example.
        /// </summary>
        /// <param name="helper">NanolibHelper instance.</param>
        /// <param name="deviceHandle">Nlc.DeviceHandle on which sampler will be executed.</param>
        public NanolibSamplerExample(NanolibHelper helper, Nlc.DeviceHandle deviceHandle)
        {            
            this.helper = helper;
            sampler = helper.GetSamplerInterface();
            this.deviceHandle = deviceHandle;
        }

        /// <summary>
        /// Run sampler examples.
        /// </summary>
        public void RunSamplerExamples()
        {
            SamplerExamplerWithoutNotification();
            SamplerExamplesWithNotification();
        }

        private void HandleSamplerFailed()
        {
            Nlc.ResultVoid lastError = sampler.getLastError(deviceHandle);

            Console.Error.WriteLine("Sampler execution failed with error: {0}", lastError.getError());
        }

        /// <summary>
        /// Examples without notification(callback).
        /// </summary>
        private void SamplerExamplerWithoutNotification()
        {
            ProcessSamplerWithoutNotificationNormal();
            ProcessSamplerWithoutNotificationRepetative();
            ProcessSamplerWithoutNotificationContinuous();
        }

        /// <summary>
        /// Examples with notification(callback).
        /// </summary>
        private void SamplerExamplesWithNotification()
        {
            ProcessSamplerWithNotificationNormal();
            ProcessSamplerWithNotificationRepetative();
            ProcessSamplerWithNotificationContinuous();
        }

        /// <summary>
        /// Sampler example in normal mode without notification.
        /// </summary>
        private void ProcessSamplerWithoutNotificationNormal()
        {
            Console.WriteLine("Sampler without notification in normal mode:");

            SetSamplerConfiguration(Nlc.SamplerMode.Normal);

            StartSampler(null);

            Nlc.SamplerState state;

            do
            {
                Thread.Sleep(100);
                ProcessSampledData(GetSampledData());
                state = GetSamplerState();
            } while (Nlc.SamplerState.Ready == state || Nlc.SamplerState.Running == state);

            ProcessSampledData(GetSampledData()); // Get last remaining data
        }

        /// <summary>
        /// Sampler example in repetative mode without notification.
        /// </summary>
        private void ProcessSamplerWithoutNotificationRepetative()
        {
            Console.WriteLine("Sampler without notification in repetative mode:");

            SetSamplerConfiguration(Nlc.SamplerMode.Repetitive);

            StartSampler(null);

            Nlc.SamplerState state = Nlc.SamplerState.Ready;

            do
            {
                Thread.Sleep(100);
                ProcessSampledData(GetSampledData());

                if (lastIteration >= 4)
                {
                    // In repeat mode the sampler will continue to run until it is stopped or an error occurs
                    sampler.stop(deviceHandle);
                    break;
                }

                state = GetSamplerState();
            } while (Nlc.SamplerState.Ready == state || Nlc.SamplerState.Running == state);

            ProcessSampledData(GetSampledData()); // Get last remaining data

            if (state == Nlc.SamplerState.Failed)
                HandleSamplerFailed();
        }

        /// <summary>
        /// Sample example in continuous mode without notification.
        /// </summary>
        private void ProcessSamplerWithoutNotificationContinuous()
        {
            Console.WriteLine("Sampler without notification in continuous mode:");
            SetSamplerConfiguration(Nlc.SamplerMode.Continuous);

            StartSampler(null);

            Nlc.SamplerState state = Nlc.SamplerState.Ready;

            int seconds = 0;

            do
            {
                Thread.Sleep(1000);
                seconds++;
                ProcessSampledData(GetSampledData());

                if (seconds >= 10)
                {
                    // In continuous mode the sampler will continue to run until it is stopped or an error occurs
                    sampler.stop(deviceHandle);
                    break;
                }

                state = GetSamplerState();
            } while (Nlc.SamplerState.Ready == state || Nlc.SamplerState.Running == state);

            ProcessSampledData(GetSampledData()); // Get last remaining data

            if (state == Nlc.SamplerState.Failed)
                HandleSamplerFailed();
        }

        /// <summary>
        /// Sampler example in normal node with notification.
        /// </summary>
        private void ProcessSamplerWithNotificationNormal()
        {
            Console.WriteLine("Sampler with notification in normal mode:");

            SetSamplerConfiguration(Nlc.SamplerMode.Normal);

            // Create sampler notification callback
            SamplerNotifyCallback callback = new SamplerNotifyCallback(ProcessSampledData);

            StartSampler(callback);

            callback.Wait();
        }

        /// <summary>
        /// Sampler example in repetative mode with notification.
        /// </summary>
        private void ProcessSamplerWithNotificationRepetative()
        {
            Console.WriteLine("Sampler with notification in repetative mode:");

            SetSamplerConfiguration(Nlc.SamplerMode.Repetitive);

            // Create sampler notification callback
            SamplerNotifyCallback callback = new SamplerNotifyCallback(ProcessSampledData);

            StartSampler(callback);

            while (!callback.Wait(100))
            {
                if (lastIteration >= 4)
                {
                    // In repeat mode the sampler will continue to run until it is stopped or an error occurs
                    sampler.stop(deviceHandle);
                }
            }
        }

        /// <summary>
        /// Sampler example in cotinuous mode with notification.
        /// </summary>
        private void ProcessSamplerWithNotificationContinuous()
        {
            Console.WriteLine("Sampler with notification in continuous mode:");
            SetSamplerConfiguration(Nlc.SamplerMode.Continuous);

            // Create sampler notification callback
            SamplerNotifyCallback callback = new SamplerNotifyCallback(ProcessSampledData);

            StartSampler(callback);

            Thread.Sleep(10000);
            sampler.stop(deviceHandle); // In continuous the sampler will continue to run until it is stopped or an error occurs
            // It's now safe to destroy the notification object(SamplerNotifyCallback callback)
        }

        /// <summary>
        /// Set sampler configuration.
        /// </summary>
        /// <param name="mode">Sampler mode.</param>
        private void SetSamplerConfiguration(Nlc.SamplerMode mode)
        {
            configuration.numberOfSamples = 5;
            configuration.periodMilliseconds = 1000;
            configuration.preTriggerNumberOfSamples = 0; // Unused currently
            configuration.trackedAddresses = trackedAddresses;
            configuration.triggerAddress = triggerAddress;
            configuration.triggerCondition = Nlc.SamplerTriggerCondition.TC_GREATER;
            configuration.triggerValue = 10;
            configuration.forceSoftwareImplementation = Nlc.SamplerMode.Continuous == mode;            
            configuration.mode = mode;

            Nlc.ResultVoid result = sampler.configure(deviceHandle, configuration);
            if (result.hasError())
            {
                throw new NanolibException("Unable to configure the sampler: " + result.getError());
            }
        }

        /// <summary>
        /// Starts the sampler.
        /// </summary>
        /// <param name="notify"></param>
        private void StartSampler(Nlc.SamplerNotify notify, Int64 applicationData = 0)
        {
            headerPrinted = false;
            sampleNumber = 0;
            lastIteration = ulong.MaxValue;

            if (trackedAddresses.Count < 1 || trackedAddresses.Count > 12)
            {
                throw new InvalidOperationException("The number of tracked addresses must be between 1 and 12");
            }

            if (trackedAddresses.Count != addressNames.Length)
            {
                throw new InvalidOperationException("The number of tracked addresses is not equal to the number of address names");
            }

            helper.WriteNumber(deviceHandle, 0, triggerAddress, 32); // Reset the trigger

            Nlc.ResultVoid result = sampler.start(deviceHandle, notify, applicationData);
            if (result.hasError())
            {
                throw new NanolibException("Unable to start the sampler: " + result.getError());
            }

            helper.WriteNumber(deviceHandle, 11, triggerAddress, 32); // Fire the trigger
        }

        /// <summary>
        /// Process the sampled data.
        /// </summary>
        /// <param name="sampleDatas">Sampled data</param>
        private void ProcessSampledData(Nlc.SampleDataVector sampleDatas)
        {
            if (sampleDatas?.Count > 0)
            {
                foreach (var sampleData in sampleDatas)
                {
                    if (sampleData.sampledValues.Count % addressNames.Length != 0)
                    {
                        throw new InvalidOperationException("SamplerNotifyCallback is not configured properly");
                    }

                    if (lastIteration != sampleData.iterationNumber)
                    {
                        lastIteration = sampleData.iterationNumber;
                        sampleNumber = 0;
                    }

                    if (!headerPrinted)
                    {
                        Console.WriteLine("------------------------------------------------------------");
                        Console.Write("{0,-11}{1,-9}", "Interation", "Sample");
                        foreach (var addressName in addressNames)
                        {
                            Console.Write("{0,-14}{1,-8}", string.Format("[{0}]", addressName), "Time");
                        }
                        Console.WriteLine();
                        Console.WriteLine("------------------------------------------------------------");
                        headerPrinted = true;
                    }

                    for (int i = 0; i < sampleData.sampledValues.Count; i += addressNames.Length)
                    {
                        StringBuilder output = new StringBuilder();
                        output.Append(String.Format("{0,-11}{1,-9}", sampleData.iterationNumber, sampleNumber++));
                        for (int j = 0; j < addressNames.Length; j++)
                        {
                            output.Append(String.Format("{0,-14}{1,-8}", sampleData.sampledValues[i + j].value, sampleData.sampledValues[i + j].collectTimeMsec));
                        }

                        Console.WriteLine(output.ToString());
                    }
                }
            }
        }

        /// <summary>
        /// Gets actual sampler state.
        /// </summary>
        /// <returns></returns>
        private Nlc.SamplerState GetSamplerState()
        {
            Nlc.ResultSamplerState resultState = sampler.getState(deviceHandle);

            if (resultState.hasError())
            {
                throw new NanolibException("Unable to get sampler state: " + resultState.getError());
            }

            return resultState.getResult();
        }

        /// <summary>
        /// Gets buffered sampled data.
        /// </summary>
        /// <returns></returns>
        private Nlc.SampleDataVector GetSampledData()
        {
            Nlc.ResultSampleDataArray resultData = sampler.getData(deviceHandle);

            if (resultData.hasError())
            {
                throw new NanolibException("Unable to get sampler data: " + resultData.getError());
            }

            return resultData.getResult();
        }
    }
}
