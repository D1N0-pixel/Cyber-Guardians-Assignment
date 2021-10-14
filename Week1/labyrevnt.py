from idautils import *
from idaapi import *
from idc import *
import ida_bytes
import ctypes

walk_end = get_name_ea_simple("walk_end")
func_addr = walk_end
flag = str()

for i in range(1000):
    xref = next(XrefsTo(func_addr)).frm
    mov = prev_head(xref)
    if (len(list(XrefsTo(mov)))):
        jmp = next(XrefsTo(mov)).frm
    else:
        jmp = prev_head(mov)
    cmp_chr = prev_head(jmp)
    if (print_insn_mnem(cmp_chr) != "cmp"):
        tmp = cmp_chr
        while print_insn_mnem(tmp) != "lea":
            tmp = prev_head(tmp)
        data = get_operand_value(tmp, 1)
        j = 0
        while data + ctypes.c_int(ida_bytes.get_dword(data + j*4)).value != mov:
            j += 1
        while print_insn_mnem(tmp) != "sub":
            tmp = prev_head(tmp)
        cmp_chr = tmp
        flag = chr(get_operand_value(cmp_chr, 1) + j) + flag
    else:
        flag = chr(get_operand_value(cmp_chr, 1)) + flag
    func_addr = next(Functions(cmp_chr))
    if get_func_name(func_addr) == "walk_start":
        break

print(flag)
    
# aBIksNPZlfMnluFMRqtNOAkdWfuMuTIICGGWvhbWYwMlbdlCGznVNVzAsHjynOjHuuuvMkOmLMhYVeEWKjGLhmhLxyvtvxpzGCWuibxDhGzEmAfkepZDINxdHTQkKrirkJNnmyVRweEjBoEAwgTVEEkEVdRjzAFcxZrdSYbPQstuILsIjOSWgLLLXvkCAQVyYqJxa

