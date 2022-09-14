// <copyright file="OdIndex.cs" company="Nanotec">
// Copyright (c) Nanotec. All rights reserved.
// </copyright>

namespace Nlc
{
    /// <summary>
    /// Struct OdIndex containing Index:SubIndex pair.
    /// It is passed to Nanolib by reference.
    /// </summary>
    public struct OdIndex
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="OdIndex"/> struct.
        /// </summary>
        /// <param name="index">Index value.</param>
        /// <param name="subIndex">Sub index value.</param>
        public OdIndex(ushort index, byte subIndex)
        {
            Index = index;
            SubIndex = subIndex;
        }

        /// <summary>
        /// Gets Index value.
        /// </summary>
        public ushort Index { get; }

        /// <summary>
        /// Gets SubIndex value.
        /// </summary>
        public byte SubIndex { get; }

        /// <summary>
        /// Returns string representation of the Index:SubIndex pair.
        /// </summary>
        /// <returns>string.</returns>
        public override string ToString()
        {
            return "0x" + Index.ToString("X4") + ":0x" + SubIndex.ToString("X2");
        }
    }
}