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

public class NlcScanBusCallback : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal NlcScanBusCallback(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(NlcScanBusCallback obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~NlcScanBusCallback() {
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
          NanolibPINVOKE.delete_NlcScanBusCallback(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public virtual ResultVoid callback(BusScanInfo info, DeviceIdVector devicesFound, int data) {
    ResultVoid ret = new ResultVoid(NanolibPINVOKE.NlcScanBusCallback_callback(swigCPtr, (int)info, DeviceIdVector.getCPtr(devicesFound), data), true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public NlcScanBusCallback() : this(NanolibPINVOKE.new_NlcScanBusCallback(), true) {
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("callback", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateNlcScanBusCallback_0(SwigDirectorMethodcallback);
    NanolibPINVOKE.NlcScanBusCallback_director_connect(swigCPtr, swigDelegate0);
  }

  private bool SwigDerivedClassHasMethod(string methodName, global::System.Type[] methodTypes) {
    global::System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, global::System.Reflection.BindingFlags.Public | global::System.Reflection.BindingFlags.NonPublic | global::System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(NlcScanBusCallback));
    return hasDerivedMethod;
  }

  private global::System.IntPtr SwigDirectorMethodcallback(int info, global::System.IntPtr devicesFound, int data) {
    return ResultVoid.getCPtr(callback((BusScanInfo)info, new DeviceIdVector(devicesFound, false), data)).Handle;
  }

  public delegate global::System.IntPtr SwigDelegateNlcScanBusCallback_0(int info, global::System.IntPtr devicesFound, int data);

  private SwigDelegateNlcScanBusCallback_0 swigDelegate0;

  private static global::System.Type[] swigMethodTypes0 = new global::System.Type[] { typeof(BusScanInfo), typeof(DeviceIdVector), typeof(int) };
}

}
