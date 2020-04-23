import ctypes

lib = ctypes.cdll.LoadLibrary("./mainLib.so")
print(lib.test())
