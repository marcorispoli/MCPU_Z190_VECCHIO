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

public class CanOpenNmtState : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CanOpenNmtState(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(CanOpenNmtState obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~CanOpenNmtState() {
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
          NanolibPINVOKE.delete_CanOpenNmtState(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public string STOPPED {
    get {
      string ret = NanolibPINVOKE.CanOpenNmtState_STOPPED_get(swigCPtr);
      return ret;
    } 
  }

  public string PRE_OPERATIONAL {
    get {
      string ret = NanolibPINVOKE.CanOpenNmtState_PRE_OPERATIONAL_get(swigCPtr);
      return ret;
    } 
  }

  public string OPERATIONAL {
    get {
      string ret = NanolibPINVOKE.CanOpenNmtState_OPERATIONAL_get(swigCPtr);
      return ret;
    } 
  }

  public string INITIALIZATION {
    get {
      string ret = NanolibPINVOKE.CanOpenNmtState_INITIALIZATION_get(swigCPtr);
      return ret;
    } 
  }

  public string UNKNOWN {
    get {
      string ret = NanolibPINVOKE.CanOpenNmtState_UNKNOWN_get(swigCPtr);
      return ret;
    } 
  }

  public CanOpenNmtState() : this(NanolibPINVOKE.new_CanOpenNmtState(), true) {
  }

}

}