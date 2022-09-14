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

public class ResultObjectDictionary : Result {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal ResultObjectDictionary(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NanolibPINVOKE.ResultObjectDictionary_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ResultObjectDictionary obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NanolibPINVOKE.delete_ResultObjectDictionary(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public ResultObjectDictionary(ObjectDictionary result_) : this(NanolibPINVOKE.new_ResultObjectDictionary__SWIG_0(ObjectDictionary.getCPtr(result_)), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public ResultObjectDictionary(string errorString_) : this(NanolibPINVOKE.new_ResultObjectDictionary__SWIG_1(errorString_), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public ResultObjectDictionary(NlcErrorCode errCode, string errorString_) : this(NanolibPINVOKE.new_ResultObjectDictionary__SWIG_2((int)errCode, errorString_), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public ResultObjectDictionary(NlcErrorCode errCode, uint exErrCode, string errorString_) : this(NanolibPINVOKE.new_ResultObjectDictionary__SWIG_3((int)errCode, exErrCode, errorString_), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public ResultObjectDictionary(Result result) : this(NanolibPINVOKE.new_ResultObjectDictionary__SWIG_4(Result.getCPtr(result)), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public ObjectDictionary getResult() {
    ObjectDictionary ret = new ObjectDictionary(NanolibPINVOKE.ResultObjectDictionary_getResult(swigCPtr), false);
    return ret;
  }

}

}
