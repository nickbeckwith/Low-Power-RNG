def Multiply(stream1, stream2):
    """Multiplies stream1 and stream2 together. Assumes equal length"""
    stream_out = []
    for int1, int2 in zip(stream1, stream2):
        stream_out.append(int1 == int2)
    return stream_out

def Add(stream1, stream2, selectStream):
    """Adds two bit streams together. Assumes equal length"""
    stream_out = []
    for int1, int2, sel in zip(stream1, stream2, selectStream):
        stream_.out.append(sel ? int1 : int2);
    return stream_out;
