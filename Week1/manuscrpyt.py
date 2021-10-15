from idautils import *
from idaapi import *
from idc import *

def sub_10003b00(data):
    data = list(data.encode())
    for i in range(len(data)):
        if data[i] < 105 or data[i] > 112:
            if data[i] < 114 or data[i] > 121:
                if data[i] < 73 or data[i] > 80:
                    if data[i] >= 82 and data[i] <= 89:
                        data[i] -= 9
                else:
                    data[i] += 9
            else:
                data[i] -= 9
        else:
            data[i] += 9
    return ''.join(map(chr, data))

def find_function_arg(addr):
    for i in range(100):
        addr = prev_head(addr)
        if print_insn_mnem(addr)[:4] == "push":
            return get_operand_value(addr, 0)
    return ""

def get_string(addr):
    out = ""
    while True:
        if get_wide_byte(addr) != 0:
            out += chr(get_wide_byte(addr))
        else:
            break
        addr += 1
    return out

for x in XrefsTo(0x10003B00, flags=0):
    enc = get_string(find_function_arg(x.frm))
    deobf = sub_10003b00(enc)
    set_cmt(x.frm, deobf, 0)

    ct = decompile(x.frm)
    tl = treeloc_t()
    tl.ea = x.frm
    tl.itp = ITP_SEMI
    ct.set_user_cmt(tl, deobf)
    ct.save_user_cmts()

