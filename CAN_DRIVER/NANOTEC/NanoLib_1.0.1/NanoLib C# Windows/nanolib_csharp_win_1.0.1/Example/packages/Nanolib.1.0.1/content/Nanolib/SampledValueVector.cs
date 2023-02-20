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

public class SampledValueVector : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<SampledValue>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal SampledValueVector(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(SampledValueVector obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~SampledValueVector() {
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
          NanolibPINVOKE.delete_SampledValueVector(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public SampledValueVector(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SampledValue element in c) {
      this.Add(element);
    }
  }

  public SampledValueVector(global::System.Collections.Generic.IEnumerable<SampledValue> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SampledValue element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public SampledValue this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < size())
        throw new global::System.ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(SampledValue[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(SampledValue[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, SampledValue[] array, int arrayIndex, int count)
  {
    if (array == null)
      throw new global::System.ArgumentNullException("array");
    if (index < 0)
      throw new global::System.ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new global::System.ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new global::System.ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new global::System.ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new global::System.ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  public SampledValue[] ToArray() {
    SampledValue[] array = new SampledValue[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<SampledValue> global::System.Collections.Generic.IEnumerable<SampledValue>.GetEnumerator() {
    return new SampledValueVectorEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new SampledValueVectorEnumerator(this);
  }

  public SampledValueVectorEnumerator GetEnumerator() {
    return new SampledValueVectorEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class SampledValueVectorEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<SampledValue>
  {
    private SampledValueVector collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public SampledValueVectorEnumerator(SampledValueVector collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public SampledValue Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (SampledValue)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object global::System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new global::System.InvalidOperationException("Collection modified.");
      }
    }

    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
  }

  public void Clear() {
    NanolibPINVOKE.SampledValueVector_Clear(swigCPtr);
  }

  public void Add(SampledValue x) {
    NanolibPINVOKE.SampledValueVector_Add(swigCPtr, SampledValue.getCPtr(x));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = NanolibPINVOKE.SampledValueVector_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = NanolibPINVOKE.SampledValueVector_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    NanolibPINVOKE.SampledValueVector_reserve(swigCPtr, n);
  }

  public SampledValueVector() : this(NanolibPINVOKE.new_SampledValueVector__SWIG_0(), true) {
  }

  public SampledValueVector(SampledValueVector other) : this(NanolibPINVOKE.new_SampledValueVector__SWIG_1(SampledValueVector.getCPtr(other)), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public SampledValueVector(int capacity) : this(NanolibPINVOKE.new_SampledValueVector__SWIG_2(capacity), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  private SampledValue getitemcopy(int index) {
    SampledValue ret = new SampledValue(NanolibPINVOKE.SampledValueVector_getitemcopy(swigCPtr, index), true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private SampledValue getitem(int index) {
    SampledValue ret = new SampledValue(NanolibPINVOKE.SampledValueVector_getitem(swigCPtr, index), false);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, SampledValue val) {
    NanolibPINVOKE.SampledValueVector_setitem(swigCPtr, index, SampledValue.getCPtr(val));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(SampledValueVector values) {
    NanolibPINVOKE.SampledValueVector_AddRange(swigCPtr, SampledValueVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public SampledValueVector GetRange(int index, int count) {
    global::System.IntPtr cPtr = NanolibPINVOKE.SampledValueVector_GetRange(swigCPtr, index, count);
    SampledValueVector ret = (cPtr == global::System.IntPtr.Zero) ? null : new SampledValueVector(cPtr, true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, SampledValue x) {
    NanolibPINVOKE.SampledValueVector_Insert(swigCPtr, index, SampledValue.getCPtr(x));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, SampledValueVector values) {
    NanolibPINVOKE.SampledValueVector_InsertRange(swigCPtr, index, SampledValueVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    NanolibPINVOKE.SampledValueVector_RemoveAt(swigCPtr, index);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    NanolibPINVOKE.SampledValueVector_RemoveRange(swigCPtr, index, count);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public static SampledValueVector Repeat(SampledValue value, int count) {
    global::System.IntPtr cPtr = NanolibPINVOKE.SampledValueVector_Repeat(SampledValue.getCPtr(value), count);
    SampledValueVector ret = (cPtr == global::System.IntPtr.Zero) ? null : new SampledValueVector(cPtr, true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    NanolibPINVOKE.SampledValueVector_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    NanolibPINVOKE.SampledValueVector_Reverse__SWIG_1(swigCPtr, index, count);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, SampledValueVector values) {
    NanolibPINVOKE.SampledValueVector_SetRange(swigCPtr, index, SampledValueVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

}

}