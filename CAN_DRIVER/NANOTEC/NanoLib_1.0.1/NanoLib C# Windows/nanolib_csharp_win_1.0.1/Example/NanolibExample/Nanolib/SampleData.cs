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

public class SampleData : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal SampleData(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(SampleData obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~SampleData() {
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
          NanolibPINVOKE.delete_SampleData(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public ulong iterationNumber {
    set {
      NanolibPINVOKE.SampleData_iterationNumber_set(swigCPtr, value);
    } 
    get {
      ulong ret = NanolibPINVOKE.SampleData_iterationNumber_get(swigCPtr);
      return ret;
    } 
  }

  public SampledValueVector sampledValues {
    set {
      NanolibPINVOKE.SampleData_sampledValues_set(swigCPtr, SampledValueVector.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = NanolibPINVOKE.SampleData_sampledValues_get(swigCPtr);
      SampledValueVector ret = (cPtr == global::System.IntPtr.Zero) ? null : new SampledValueVector(cPtr, false);
      return ret;
    } 
  }

  public SampleData() : this(NanolibPINVOKE.new_SampleData(), true) {
  }

}

}