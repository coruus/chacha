from cffi import FFI

_ffi = FFI()
_ffi.cdef("\n".join(line for line in open("chacha.h").readlines() if not line.startswith("#")))
_chacha = _ffi.dlopen("./ref/chacha.dylib")


def chacha_test(key='\x00' * 32, nonce='\x00' * 8):
    out = _ffi.new('uint8_t[128]')
    zeros = _ffi.new('uint8_t[128]')
    _chacha.chacha8_xor(out, zeros, len(out), key, nonce)
    return _ffi.buffer(out)[:]

s = chacha_test()
