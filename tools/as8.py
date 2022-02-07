#!/usr/bin/env python3

import argparse

parser = argparse.ArgumentParser(description="chip8 assembler")
parser.add_argument("input", type=str, help="chip8 assembly file")
parser.add_argument("output", type=str, help="chip8 code output")

args = parser.parse_args()

asmfile = args.input
binfile = args.output

memsize = 0xffff


def valid_inst(inst, name, length):
    return inst[0] == name and len(inst) == length


def get_reg(reg):
    l = list(reg)
    if l[0] != 'V' or len(l) != 2:
        raise "invalid register"
    return int(l[1], base=16)


def get_nbits(val, n):
    v = int(val, base=0)
    if v >> n != 0:
        raise f"invalid {n} bit integer: {val}"
    return v


def translate(inst):
    if valid_inst(inst, "cls", 1):
        return 0x00e0
    elif valid_inst(inst, "ret", 1):
        return 0x00ee
    elif valid_inst(inst, "jp", 2):
        addr = get_nbits(inst[1], 3*4)
        return 0x1000 | addr
    elif valid_inst(inst, "call", 2):
        addr = get_nbits(inst[1], 3*4)
        return 0x2000 | addr
    elif valid_inst(inst, "se", 3):
        reg = get_reg(inst[1])
        val = get_nbits(inst[2], 2*4)
        return 0x3000 | reg << 8 | val
    elif valid_inst(inst, "sne", 3):
        reg = get_reg(inst[1])
        val = get_nbits(inst[2], 2*4)
        return 0x4000 | reg << 8 | val
    elif valid_inst(inst, "ser", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x5000 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "ld", 3):
        reg = get_reg(inst[1])
        val = get_nbits(inst[1], 2*4)
        return 0x6000 | reg << 8 | val
    elif valid_inst(inst, "add", 3):
        reg = get_reg(inst[1])
        val = get_nbits(inst[2], 2*4)
        return 0x7000 | reg << 8 | val
    elif valid_inst(inst, "ldr", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8000 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "or", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8001 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "and", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8002 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "xor", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8003 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "addc", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8004 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "sub", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8005 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "shr", 3):
        reg1 = get_reg(inst[1])
        return 0x8006 | reg1 << 8
    elif valid_inst(inst, "subn", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x8007 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "shl", 3):
        reg1 = get_reg(inst[1])
        return 0x8006 | reg1 << 8
    elif valid_inst(inst, "sner", 3):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        return 0x9000 | reg1 << 8 | reg2 << 4
    elif valid_inst(inst, "ldi", 2):
        addr = get_nbits(inst[1], 3*4)
        return 0xa000 | addr
    elif valid_inst(inst, "jp0", 2):
        addr = get_nbits(inst[1], 3*4)
        return 0xb000 | addr
    elif valid_inst(inst, "rnd", 3):
        reg = get_reg(inst[1])
        val = get_nbits(inst[2], 2*4)
        return 0xc000 | reg << 8 | val
    elif valid_inst(inst, "drw", 4):
        reg1 = get_reg(inst[1])
        reg2 = get_reg(inst[2])
        n = get_nbits(inst[3], 4)
        return 0xd000 | reg1 << 8 | reg2 << 4 | n
    else:
        raise f"{inst}: instruction invalid or not implemented"


with open(binfile, "wb") as bf:
    with open(asmfile) as af:
        for l in af:
            inst = l.strip().split()

            try:
                code = translate(inst)
                bf.write(code.to_bytes(2, byteorder="big"))
            except Exception as e:
                print(e)
                exit()
