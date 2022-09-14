// <copyright file="DeviceHandle.cs" company="Nanotec">
// Copyright (c) Nanotec. All rights reserved.
// </copyright>

namespace Nlc
{
    /// <summary>
    /// Struct DeviceHandle containing device handle for NanoLibAccessor.
    /// It is passed to Nanolib by value.
    /// </summary>
    public struct DeviceHandle
    {
        /// <summary>
        /// Holds handle value.
        /// </summary>
        private readonly uint handle;

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceHandle"/> struct.
        /// </summary>
        /// <param name="deviceHandle">Device handle.</param>
        public DeviceHandle(DeviceHandle deviceHandle)
        {
            handle = deviceHandle.handle;
        }
    }
}