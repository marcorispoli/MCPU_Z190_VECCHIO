//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 4.0.2
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace  Nlc {

public class BusHardwareId : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal BusHardwareId(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(BusHardwareId obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~BusHardwareId() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NanolibPINVOKE.delete_BusHardwareId(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public BusHardwareId(string busHardware_, string protocol_, string hardwareSpecifier_, string name_) : this(NanolibPINVOKE.new_BusHardwareId__SWIG_0(busHardware_, protocol_, hardwareSpecifier_, name_), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public BusHardwareId(string busHardware_, string protocol_, string hardwareSpecifier_, string extraHardwareSpecifier_, string name_) : this(NanolibPINVOKE.new_BusHardwareId__SWIG_1(busHardware_, protocol_, hardwareSpecifier_, extraHardwareSpecifier_, name_), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public BusHardwareId(BusHardwareId arg0) : this(NanolibPINVOKE.new_BusHardwareId__SWIG_2(BusHardwareId.getCPtr(arg0)), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public BusHardwareId() : this(NanolibPINVOKE.new_BusHardwareId__SWIG_3(), true) {
  }

  public string getBusHardware() {
    string ret = NanolibPINVOKE.BusHardwareId_getBusHardware(swigCPtr);
    return ret;
  }

  public string getProtocol() {
    string ret = NanolibPINVOKE.BusHardwareId_getProtocol(swigCPtr);
    return ret;
  }

  public string getHardwareSpecifier() {
    string ret = NanolibPINVOKE.BusHardwareId_getHardwareSpecifier(swigCPtr);
    return ret;
  }

  public string getExtraHardwareSpecifier() {
    string ret = NanolibPINVOKE.BusHardwareId_getExtraHardwareSpecifier(swigCPtr);
    return ret;
  }

  public string getName() {
    string ret = NanolibPINVOKE.BusHardwareId_getName(swigCPtr);
    return ret;
  }

  public bool equals(BusHardwareId other) {
    bool ret = NanolibPINVOKE.BusHardwareId_equals(swigCPtr, BusHardwareId.getCPtr(other));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string toString() {
    string ret = NanolibPINVOKE.BusHardwareId_toString(swigCPtr);
    return ret;
  }

}

}