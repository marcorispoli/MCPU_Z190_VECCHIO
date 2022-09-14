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

public class SampleDataVector : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<SampleData>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal SampleDataVector(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(SampleDataVector obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~SampleDataVector() {
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
          NanolibPINVOKE.delete_SampleDataVector(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public SampleDataVector(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SampleData element in c) {
      this.Add(element);
    }
  }

  public SampleDataVector(global::System.Collections.Generic.IEnumerable<SampleData> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (SampleData element in c) {
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

  public SampleData this[int index]  {
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

  public void CopyTo(SampleData[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(SampleData[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, SampleData[] array, int arrayIndex, int count)
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

  public SampleData[] ToArray() {
    SampleData[] array = new SampleData[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<SampleData> global::System.Collections.Generic.IEnumerable<SampleData>.GetEnumerator() {
    return new SampleDataVectorEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new SampleDataVectorEnumerator(this);
  }

  public SampleDataVectorEnumerator GetEnumerator() {
    return new SampleDataVectorEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class SampleDataVectorEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<SampleData>
  {
    private SampleDataVector collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public SampleDataVectorEnumerator(SampleDataVector collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public SampleData Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (SampleData)currentObject;
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
    NanolibPINVOKE.SampleDataVector_Clear(swigCPtr);
  }

  public void Add(SampleData x) {
    NanolibPINVOKE.SampleDataVector_Add(swigCPtr, SampleData.getCPtr(x));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = NanolibPINVOKE.SampleDataVector_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = NanolibPINVOKE.SampleDataVector_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    NanolibPINVOKE.SampleDataVector_reserve(swigCPtr, n);
  }

  public SampleDataVector() : this(NanolibPINVOKE.new_SampleDataVector__SWIG_0(), true) {
  }

  public SampleDataVector(SampleDataVector other) : this(NanolibPINVOKE.new_SampleDataVector__SWIG_1(SampleDataVector.getCPtr(other)), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public SampleDataVector(int capacity) : this(NanolibPINVOKE.new_SampleDataVector__SWIG_2(capacity), true) {
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  private SampleData getitemcopy(int index) {
    SampleData ret = new SampleData(NanolibPINVOKE.SampleDataVector_getitemcopy(swigCPtr, index), true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private SampleData getitem(int index) {
    SampleData ret = new SampleData(NanolibPINVOKE.SampleDataVector_getitem(swigCPtr, index), false);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, SampleData val) {
    NanolibPINVOKE.SampleDataVector_setitem(swigCPtr, index, SampleData.getCPtr(val));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(SampleDataVector values) {
    NanolibPINVOKE.SampleDataVector_AddRange(swigCPtr, SampleDataVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public SampleDataVector GetRange(int index, int count) {
    global::System.IntPtr cPtr = NanolibPINVOKE.SampleDataVector_GetRange(swigCPtr, index, count);
    SampleDataVector ret = (cPtr == global::System.IntPtr.Zero) ? null : new SampleDataVector(cPtr, true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, SampleData x) {
    NanolibPINVOKE.SampleDataVector_Insert(swigCPtr, index, SampleData.getCPtr(x));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, SampleDataVector values) {
    NanolibPINVOKE.SampleDataVector_InsertRange(swigCPtr, index, SampleDataVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    NanolibPINVOKE.SampleDataVector_RemoveAt(swigCPtr, index);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    NanolibPINVOKE.SampleDataVector_RemoveRange(swigCPtr, index, count);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public static SampleDataVector Repeat(SampleData value, int count) {
    global::System.IntPtr cPtr = NanolibPINVOKE.SampleDataVector_Repeat(SampleData.getCPtr(value), count);
    SampleDataVector ret = (cPtr == global::System.IntPtr.Zero) ? null : new SampleDataVector(cPtr, true);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    NanolibPINVOKE.SampleDataVector_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    NanolibPINVOKE.SampleDataVector_Reverse__SWIG_1(swigCPtr, index, count);
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, SampleDataVector values) {
    NanolibPINVOKE.SampleDataVector_SetRange(swigCPtr, index, SampleDataVector.getCPtr(values));
    if (NanolibPINVOKE.SWIGPendingException.Pending) throw NanolibPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
