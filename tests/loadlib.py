import ctypes 


l = ctypes.CDLL("./libbackend.so")

r = l["main"]()


print "main returned:", r

if r != 0:
    raise Exception("returned not 0")

print "SUCCESS"